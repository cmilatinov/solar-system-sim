#version 400 core

const int kernelSize = 5;

in vec2 textureCoords[(kernelSize * 2) + 1];

uniform sampler2D tex;

const float factors[] = {0.0093, 0.028002, 0.065984, 0.121703, 0.175713, 0.198596, 0.175713, 0.121703, 0.065984, 0.028002, 0.0093};

void main(void){
	vec4 color = vec4(0.0);
	for(int i = 0; i < (kernelSize * 2) + 1; i++){
		color += texture(tex, textureCoords[i]) * factors[i];
	}
	gl_FragColor = color;
}