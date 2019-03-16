#version 400 core

const float transparency = 0.85;

in vec3 finalColour;
in vec3 specularColor;
in vec4 clipSpace;
in float fresnelFactor;

in vec3 toCameraVector;
in vec3 surfaceNormal;

uniform mat4 cameraViewMatrix;
uniform mat4 perspectiveProjectionMatrix;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D depthTexture;

const float near = 0.1f;
const float	far = 1000.0f;

void main(void){
	
	float finalFactor = fresnelFactor;
	finalFactor = clamp(finalFactor, 0.0, 1.0);

	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;	
	vec2 refractionCoords = ndc;
	vec2 reflectionCoords = vec2(ndc.x, -ndc.y);

	float depthSample = texture(depthTexture, refractionCoords).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depthSample - 1.0) * (far - near));
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * gl_FragCoord.z - 1.0) * (far - near));
	float depth = floorDistance - waterDistance;
	
	float actualDepth = depth;

	if(depth <= 0)
		discard;

	depth = clamp(depth / 2.0, 0.0, 1.0);

	//Refraction + Reflection distortion
	
	vec3 unitFromCamera = normalize(-toCameraVector);
	vec3 refractedLightRay = actualDepth * refract(unitFromCamera, surfaceNormal, (1.0 / 1.03));
	vec3 cameraPosition = (inverse(cameraViewMatrix) * vec4(0, 0, 0, 1)).xyz;
	vec3 worldPosition = cameraPosition - toCameraVector + refractedLightRay;
	vec4 transformedPosition = perspectiveProjectionMatrix * cameraViewMatrix * vec4(worldPosition, 1.0);
	vec2 textureCoords = (transformedPosition.xy / transformedPosition.w) / 2.0 + 0.5;

	//Making sure refraction distortion doesnt show white edge
	float refractionDepth = texture(depthTexture, vec2(textureCoords.x, refractionCoords.y)).r;
	if(refractionDepth < 0.99999999){
		refractionCoords = vec2(textureCoords.x, refractionCoords.y);
	}
	reflectionCoords = vec2(textureCoords.x, reflectionCoords.y);

	//Clamping the distortion
	vec2 clampedReflectionCoords = vec2(0.0); 
	clampedReflectionCoords.x = clamp(reflectionCoords.x, 0.001, 0.999);
	clampedReflectionCoords.y = clamp(reflectionCoords.y, -0.999, -0.001);
	vec2 clampedRefractionCoords = clamp(refractionCoords, 0.001, 0.999);

	//Sample the Refraction and Reflection textures
	vec4 reflectionColor = texture(reflectionTexture, clampedReflectionCoords);
	vec4 refractionColor = texture(refractionTexture, clampedRefractionCoords);
	if(refractionColor.x > 0.5 && refractionColor.y > 0.5 && refractionColor.z > 0.5)
		refractionColor.w = 0.0;
	//Mix the colors using the provided factors
	gl_FragColor = (mix(mix(reflectionColor, refractionColor, finalFactor), vec4(finalColour, 1.0), 0.4) * vec4(1.0, 1.0, 1.0, depth)) + vec4(specularColor, 0.0);
}