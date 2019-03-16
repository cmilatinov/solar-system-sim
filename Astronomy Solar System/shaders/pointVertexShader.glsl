#version 400 core

in vec4 pos;
in vec3 color;

out vec4 pass_Color;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void){
	vec3 toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - pos.xyz;
	pass_Color = vec4(color, min(length(toCameraVector) / pos.w, 1.0));
	gl_Position = projectionMatrix * viewMatrix * vec4(pos.xyz, 1.0);
}