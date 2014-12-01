#version 430
in vec3 Normal;
in vec2 TexCoord;

// The g-buffer textures
layout(binding = 0) uniform sampler2D NormalTex;
layout(binding = 1) uniform sampler2D ColorTex;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

layout (location = 0) out vec3 NormalData;
layout (location = 1) out vec3 ColorData;

void main() 
{
	// Store position, norm, and diffuse color in g-buffer
	NormalData.xy = Normal.xy;

	//
	//float4 normal_map;
	//normal_map = gNormal.Sample( samp, input.Tex );
	//normal_map = (normal_map * 2.0f) - 1.0f;

	//float3x3 texSpace = float3x3(input.Tan, input.BiNorm, input.Norm);//biTangent, input.Norm);
	//normal = normalize(mul(normal_map, texSpace));


	ColorData = texture( diffuseTex, TexCoord ).rgb;
}