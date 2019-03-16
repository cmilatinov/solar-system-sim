#version 400 core

in vec2 pass_UV;

uniform sampler2D tex;

void main(void){

	gl_FragColor = texture(tex, pass_UV);
}