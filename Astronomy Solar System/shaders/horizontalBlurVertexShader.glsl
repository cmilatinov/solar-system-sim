#version 400 core

const int kernelSize = 5;

in vec3 pos;

out vec2 textureCoords[(kernelSize * 2) + 1];

uniform float targetWidth;

void main(void){
	gl_Position = vec4(pos, 1.0);
	vec2 centerTexCoords = pos.xy * 0.5 + 0.5;
	float pixelSize = 1.0 / targetWidth;
	
	for(int i = -kernelSize; i <= kernelSize; i++){
		textureCoords[i + kernelSize] = centerTexCoords + vec2(pixelSize * i, 0.0);
		textureCoords[i + kernelSize] = clamp(textureCoords[i + kernelSize], 0.001, 0.999);
	}
}