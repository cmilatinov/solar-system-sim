#version 400 core

in vec2 pass_UV;

uniform sampler2D tex;

const float brightnessIntervalSize = 0.4;

void main(void){
	vec2 clampedUV = clamp(pass_UV, 0.001, 0.999);
	vec4 color = texture(tex, clampedUV);
	float brightness = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	if(brightness > 1 - brightnessIntervalSize)
		gl_FragColor = color * max((brightness + brightnessIntervalSize - 1.0) / brightnessIntervalSize, 0.0);
	else
		gl_FragColor = vec4(0);
}