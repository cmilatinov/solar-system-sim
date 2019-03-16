#version 400 core

in vec4 pass_Color;

void main(void){
//	vec2 ndcCenter = (center.xy / center.w) / 2.0 + 0.5;
//	vec2 ndcFrag = (gl_FragCoord.xy / gl_FragCoord.w) / 2.0 + 0.5;
//	if(distance(ndcCenter, ndcFrag) <= 5)
		gl_FragColor = vec4(pass_Color);
}