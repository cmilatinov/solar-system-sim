#version 400 core

in vec2 pass_UV;

uniform sampler2D tex;

void main(void){
	vec4 color = texture(tex, pass_UV);
	float brightness = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	gl_FragColor = color * brightness;
}