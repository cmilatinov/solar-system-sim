#version 400 core

const int MAX_LIGHTS = 1;

in vec3 pos;
in vec3 normal;
in vec2 uv;
in vec3 color;
in mat4 modelMatrix;

out vec2 pass_UV;
out vec3 surfaceNormal;
out vec3 pass_Color;
out vec3 toCameraVector;
out vec3 toLightVector[MAX_LIGHTS];

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
//uniform mat4 modelMatrix;
uniform vec3 lightPosition[MAX_LIGHTS];

uniform vec4 clipPlane;

void main(void){
	vec4 worldPosition = modelMatrix * vec4(pos, 1);

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);

	gl_Position = projectionMatrix * viewMatrix * worldPosition;

	for(int i = 0; i < MAX_LIGHTS; i++){
		toLightVector[i] = lightPosition[i] - worldPosition.xyz;
	}

	toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

	pass_UV = uv;
	surfaceNormal = (modelMatrix * vec4(normal, 0.0)).xyz;
	pass_Color = color;
}