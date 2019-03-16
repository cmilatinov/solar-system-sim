#version 400 core

in vec2 pass_UV;

uniform sampler2D tex;
uniform float contrast;

void main(void){
	vec2 clampedUV = clamp(pass_UV, 0.001, 0.999);
	vec4 color = texture(tex, clampedUV);
	gl_FragColor = vec4((color.rgb - 0.5) * (1.0 + contrast) + 0.5, 1.0);
}