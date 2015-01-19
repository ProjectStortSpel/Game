#version 440
in vec2 TexCoord;

uniform sampler2D diffuseTex;

void main() {
	//Do not write depth if diffuse.a < 0.5
	if( texture( diffuseTex, TexCoord ).a < 0.5 )
		discard;
}