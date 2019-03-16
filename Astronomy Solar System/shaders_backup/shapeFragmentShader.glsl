#version 400 core

uniform vec3 color;
uniform float opacity;

void main(void){
	gl_FragColor = vec4(color, opacity);
}