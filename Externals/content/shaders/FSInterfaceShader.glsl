#version 400
in vec2 TexCoord;
in vec4 addcolor;

layout( location = 0 ) out vec4 ColorData;

//Input textures
uniform sampler2D diffuseTex;


void main() 
{
	// Diffuse tex
	vec4 albedo_tex = texture( diffuseTex, TexCoord );

	vec4 coloradded;
	if( addcolor.xyz != vec3(0.0) )
		coloradded = albedo_tex * addcolor;
	else
		coloradded = albedo_tex;

	ColorData = coloradded;
}