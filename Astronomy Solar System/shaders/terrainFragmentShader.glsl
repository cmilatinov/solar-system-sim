#version 400 core

const int MAX_LIGHTS = 1;
const float ambientLighting = 0.0;
const float shineDamper = 14.0;

flat in vec3 surfaceNormal;
in vec3 pass_Color;
in vec3 toLightVector[MAX_LIGHTS];
in vec3 toCameraVector;
in float pass_Reflectivity;

uniform vec3 lightColor[MAX_LIGHTS];

void main(void){

	vec3 unitNormal = surfaceNormal; 

	vec3 unitCamera = normalize(toCameraVector);
	
	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);
	
	for(int i = 0; i < MAX_LIGHTS; i++){
		vec3 unitLight = normalize(toLightVector[i]);
		float nDotl = dot(unitLight, unitNormal);
		float brightness = max(nDotl, ambientLighting);
		totalDiffuse = totalDiffuse + (brightness * lightColor[i]);

		vec3 reflectedLightDirection = reflect(-unitLight, unitNormal);
		float specularFactor = dot(reflectedLightDirection, unitCamera);
		specularFactor = max(specularFactor, 0.0);
		specularFactor = max(pow(specularFactor, shineDamper), 0.0);
		totalSpecular = totalSpecular + (lightColor[i] * specularFactor * pass_Reflectivity);
	}
	
	vec4 textureColor = vec4(pass_Color, 1.0);
	
	gl_FragColor = vec4(totalDiffuse, 1.0) * textureColor + vec4(totalSpecular, 1.0);

	//out_Color = gl_FragColor;
}