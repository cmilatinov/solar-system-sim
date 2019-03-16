#version 400 core

in vec3 pos;
in float trailValue;

out float pass_TrailValue;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void){
	pass_TrailValue = trailValue;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);
}