#version 100
precision mediump float;

varying vec2 TexCoord;
uniform sampler2D diffuseTex;

uniform vec3 BlendColor;

void main() 
{
					// Diffuse tex
	vec4 albedo_tex = texture2D( diffuseTex, TexCoord );

	float blendFactor = 0.25;

	if( BlendColor != vec3(0.0) )
		albedo_tex.xyz = (1.0-blendFactor)*albedo_tex.xyz + blendFactor * BlendColor; 

	gl_FragColor = albedo_tex;
}