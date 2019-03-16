#version 400 core

in vec2 pass_UV;

uniform sampler2D tex;

void main(void){
	float dist = abs(length(pass_UV - vec2(0.5, 0.5)) - 0.4975);
	if(dist >= 0.003)
		discard;

	gl_FragColor = texture(tex, pass_UV) * vec4(1.0, 1.0, 1.0, 1.0 - (dist / 0.003));
}