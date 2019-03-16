#version 400 core

in vec3 pos;
in vec3 normal;
in vec2 uv;

out vec3 surfaceNormal;
out vec2 pass_UV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void){
	
	pass_UV = uv;

	surfaceNormal = (modelMatrix * vec4(normal, 0.0)).xyz;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);

}