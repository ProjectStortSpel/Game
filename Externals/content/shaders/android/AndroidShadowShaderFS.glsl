#version 100
#extension GL_OES_depth_texture : enable
#extension OES_fragment_precision_high : enable

precision highp float;

varying vec2 TexCoord;

uniform sampler2D diffuseTex;

void main() {
	//Do not write depth if diffuse.a < 0.5
	if( texture2D( diffuseTex, TexCoord ).a < 0.75 )
		discard;
}
