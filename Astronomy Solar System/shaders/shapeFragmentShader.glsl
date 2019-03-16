#version 400 core

in float pass_TrailValue;

uniform vec3 color;
uniform float opacity;
uniform bool usesTrailValues;

void main(void){
	if(usesTrailValues)
		gl_FragColor = vec4(color, pass_TrailValue);
	else
		gl_FragColor = vec4(color, opacity);
}