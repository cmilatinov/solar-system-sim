#version 400 core

in vec3 pos;
in vec2 uv;

out vec2 pass_UV;

uniform mat4 modelMatrix;

void main(void){
	gl_Position = modelMatrix * vec4(pos, 1);

	pass_UV = uv;
}