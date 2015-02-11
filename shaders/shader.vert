#version 120
attribute vec3 in_Position;
attribute vec2 in_TexMap;
attribute vec3 in_Normal;

varying vec2 ex_TexMap;
varying vec3 ex_Normal;
varying vec3 ex_ShadowTexMap;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform mat4 shadowViewProjectionMatrix;

mat4 biasMatrix=mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);
void main(void) {
	gl_Position=viewProjectionMatrix*modelMatrix*vec4(in_Position, 1.0);
    ex_TexMap = in_TexMap;
	ex_Normal = normalize((normalMatrix*vec4(in_Normal, 1.0)).xyz);
	vec4 shadowmap=(modelMatrix*vec4(in_Position, 1.0));
	shadowmap.z*=0.0005f;
	shadowmap=shadowViewProjectionMatrix*shadowmap;
	shadowmap.x=shadowmap.x/(abs(shadowmap.x)+1.0f);
	shadowmap.y=shadowmap.y/(abs(shadowmap.y)+1.0f);
	ex_ShadowTexMap=vec3((biasMatrix*shadowmap).xy,shadowmap.z);	
}