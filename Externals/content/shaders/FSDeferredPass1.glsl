#version 430
in vec3 Normal;
in vec2 TexCoord;

// The g-buffer textures
layout(binding = 0) uniform sampler2D NormalTex;
layout(binding = 1) uniform sampler2D ColorTex;

uniform sampler2D diffuseTex;

layout (location = 0) out vec3 NormalData;
layout (location = 1) out vec3 ColorData;

void main() 
{
	// Store position, norm, and diffuse color in g-buffer
	NormalData.xy = Normal.xy;
	ColorData = texture( diffuseTex, TexCoord ).rgb;
}