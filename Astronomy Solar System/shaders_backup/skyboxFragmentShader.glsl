#version 400 core

in vec3 textureCoords;

uniform samplerCube tex;

void main(void){
	gl_FragColor = texture(tex, textureCoords);
}