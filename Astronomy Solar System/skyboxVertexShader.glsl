#version 400 core

in vec3 pos;

out vec3 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void){
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);
	
	textureCoords = pos;
}