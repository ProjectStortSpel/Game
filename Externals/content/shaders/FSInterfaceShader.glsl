#version 400
in vec2 TexCoord;

layout( location = 0 ) out vec4 ColorData;

//Input textures
uniform sampler2D diffuseTex;


void main() 
{
	// Diffuse tex
	vec4 albedo_tex = texture( diffuseTex, TexCoord );

	ColorData = albedo_tex;
}