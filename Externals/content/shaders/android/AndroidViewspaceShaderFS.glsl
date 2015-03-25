#version 100

precision mediump float;

varying vec3 Normal;
varying vec3 Tan;
varying vec3 BiTan;
varying vec2 TexCoord;
varying vec3 ViewPos;

//Input textures
uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform vec3 BlendColor;

struct Pointlight {
	vec3 Position; // Light position in world coords.
	vec3 Intensity; // Diffuse intensity
	vec3 Color;
	float Range;
};
//Pointlight pointlights[1];

struct MaterialInfo {
	float Ks;
	float Shininess;
};
MaterialInfo Material;

vec3 NmNormal;

void main() 
{
	// Diffuse tex
	vec4 albedo_tex = texture2D( diffuseTex, TexCoord );

	// Normal data
	vec3 normal_map	  = texture2D( normalTex, TexCoord ).rgb;
	normal_map = (normal_map * 2.0) - 1.0;
	mat3 texSpace = mat3(Tan, BiTan, Normal);
	NmNormal = normalize( texSpace * normal_map );

	// Spec data
	vec4 specglow_map = texture2D( specularTex, TexCoord );
	Material.Ks			= specglow_map.x;
	Material.Shininess  = specglow_map.y * 254.0 + 1.0;
	float glow			= specglow_map.z;

	vec3 ambient = vec3(1.0);
	vec3 diffuse = vec3(0.0);
	vec3 spec    = vec3(0.0);
	
	float mMod = specglow_map.a*99.0 - 50.0 * floor((specglow_map.a*99.0)/50.0);
	float blendFactor = mMod/50.0;//(specTexture.a-0.5f)*2;
	vec3 AddedColor = BlendColor;
	if (specglow_map.a < 0.5)
		AddedColor = vec3(1) - BlendColor; // ANTICOLOR? Good or bad? I like

	if( BlendColor != vec3(0.0) )
		albedo_tex.xyz = (1.0-blendFactor)*albedo_tex.xyz + blendFactor * AddedColor;

	gl_FragColor = vec4(ambient + diffuse, 1.0) * albedo_tex + vec4(spec, 0.0) + vec4(normal_map, 0.0)*0.00000001;
}