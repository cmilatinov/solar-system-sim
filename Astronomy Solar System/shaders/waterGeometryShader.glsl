#version 400 core

layout ( triangles ) in;
layout ( triangle_strip, max_vertices = 3) out;

out vec3 finalColour;
out vec3 specularColor;
out vec4 clipSpace;
out float fresnelFactor;

out vec3 toCameraVector;
out vec3 surfaceNormal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

vec3 cameraPosition;

const vec3 lightDirection = normalize(vec3(0.4, -1.0, 0.8));
const vec3 waterColour = vec3(0.2823, 0.7882, 0.7333);
const vec3 lightColour = vec3(1.0, 0.6, 0.6);
const float reflectivity = 0.5;
const float shineDamper = 14.0;
const float ambientLighting = 0.3;

vec3 calculateTriangleNormal(){
	vec3 tangent = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 bitangent = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 normal = cross(tangent, bitangent);	
	return normalize(normal);
}

vec3 getCameraPosition(){
	return (inverse(viewMatrix) * vec4(0, 0, 0, 1)).xyz;
}

vec3 calculateSpecular(vec4 worldPosition, vec3 normal){
	vec3 viewVector = normalize(cameraPosition - worldPosition.xyz);
	vec3 reflectedLightDirection = reflect(lightDirection, normal);
	float specularFactor = dot(reflectedLightDirection, viewVector);
	specularFactor = max(pow(specularFactor, shineDamper), 0.0);
	return lightColour * specularFactor * reflectivity;
}

void main(void){
	cameraPosition = getCameraPosition();

	vec3 normal = calculateTriangleNormal();
	float brightness = max(dot(-lightDirection, normal), ambientLighting);
	vec3 colour = waterColour * brightness;
	
	surfaceNormal = normal;
	vec4 worldPosition = gl_in[0].gl_Position;
	vec3 toCamera = normalize(cameraPosition - worldPosition.xyz);
	toCameraVector = toCamera;
	fresnelFactor = clamp(dot(normal, toCamera), 0.0, 1.0);
	clipSpace = projectionMatrix * viewMatrix * worldPosition;
	gl_Position = clipSpace;
	finalColour = colour; 
	specularColor = calculateSpecular(worldPosition, normal);
	EmitVertex();

	surfaceNormal = normal;
	worldPosition = gl_in[1].gl_Position;
	toCamera = normalize(cameraPosition - worldPosition.xyz);
	toCameraVector = toCamera;
	fresnelFactor = clamp(dot(normal, toCamera), 0.0, 1.0);
	clipSpace = projectionMatrix * viewMatrix * worldPosition;
	gl_Position = clipSpace;
	finalColour = colour; 
	specularColor = calculateSpecular(worldPosition, normal);
	EmitVertex();

	surfaceNormal = normal;
	worldPosition = gl_in[2].gl_Position;
	toCamera = normalize(cameraPosition - worldPosition.xyz);
	toCameraVector = toCamera;
	fresnelFactor = clamp(dot(normal, toCamera), 0.0, 1.0);
	clipSpace = projectionMatrix * viewMatrix * worldPosition;
	gl_Position = clipSpace;
	finalColour = colour; 
	specularColor = calculateSpecular(worldPosition, normal);
	EmitVertex();

	EndPrimitive();
}