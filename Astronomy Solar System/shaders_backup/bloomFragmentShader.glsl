#version 400 core

in vec2 pass_UV;

uniform sampler2D sceneTexture;
uniform sampler2D bloomTexture;

void main(void){
	vec2 clampedUV = clamp(pass_UV, 0.001, 0.999);
	vec4 bloomColor = texture(bloomTexture, clampedUV);
	vec4 sceneColor = texture(sceneTexture, clampedUV);
	gl_FragColor = sceneColor + bloomColor * 1.4;
}