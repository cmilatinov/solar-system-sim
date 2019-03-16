#version 400 core

in vec2 pass_UV;
in vec3 surfaceNormal;

const float ambientLighting = 1.0;

uniform sampler2D tex;

uniform vec3 cameraDirection;

void main(void){
	
	vec3 unitNormal = normalize(surfaceNormal);
	float brightness = max(dot(cameraDirection, unitNormal), ambientLighting) * 1.5;

	vec4 texColor = texture(tex, pass_UV);

	gl_FragColor = texColor * brightness;

}