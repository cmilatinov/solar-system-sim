#version 400 core

in vec3 pos;

uniform float time;
uniform mat4 modelMatrix;

const float PI = 3.14159265358979323846;
const float amplitude = 0.4;

vec3 modifiedPosition;

float generateHeight(){
	float a = mod(2 * PI * time + (modifiedPosition.x * 16), 2 * PI);
	float b = mod(2 * PI * time + (modifiedPosition.x * modifiedPosition.z * 8), 2 * PI);
	float component1 = sin(a) * amplitude;
	float component2 = sin(b) * amplitude;
	return component1 + component2;
}

void main(void){
	modifiedPosition = (modelMatrix * vec4(pos, 1.0)).xyz;
	float height = generateHeight();
	gl_Position = modelMatrix * vec4(pos.x, pos.y + height, pos.z, 1);
}

