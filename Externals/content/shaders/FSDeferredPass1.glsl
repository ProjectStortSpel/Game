#version 430
in vec3 Normal;
in vec3 Tan;
in vec3 BiTan;
in vec2 TexCoord;

// The g-buffer textures
layout(binding = 0) uniform sampler2D NormalTex;
layout(binding = 1) uniform sampler2D ColorTex;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

layout (location = 0) out vec4 NormalData;
layout (location = 1) out vec4 ColorData;

void main() 
{
	// -- INPUTS --
	// Sample maps
	vec3 color_map = texture( diffuseTex, TexCoord ).rgb;
	vec3 normal_map = texture( normalTex, TexCoord ).rgb;
	vec3 specglow_map = texture( specularTex, TexCoord ).rgb;

	// -- OUTPUTS --
	// Set Color output
	ColorData.xyz = color_map.xyz;								// rgb = color
	ColorData.w = specglow_map.z;								// a = glow
	// Set Normal output
	normal_map = (normal_map * 2.0f) - 1.0f;
	mat3 texSpace = mat3(Tan, BiTan, Normal);
	NormalData.xy = normalize( texSpace * normal_map ).xy;		// rg = normal
	NormalData.z = specglow_map.x;								// b = spec reflec
	NormalData.w = specglow_map.y;								// a = spec shine
}