#version 430
in vec3 Normal;
in vec3 Tan;
in vec3 BiTan;
in vec2 TexCoord;
in vec3 ViewPos;

layout( location = 0 ) out vec4 ColorData;

//Input textures
uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform mat4 BiasMatrix;

struct vector3
{
	float x, y, z;
};

struct Pointlight {
	vector3 Position; // Light position in world coords.
	vector3 Intensity; // Diffuse intensity
	vector3 Color;
	float Range;
};

layout (std430, binding = 4) buffer PointLights   
{
	Pointlight	pointlights[];
};

struct MaterialInfo {
	float Ks;
	float Shininess;
};
MaterialInfo Material;

vec3 NmNormal;

void main() 
{
	// Diffuse tex
	vec4 albedo_tex = texture( diffuseTex, TexCoord );

	// Normal data
	vec3 normal_map	  = texture( normalTex, TexCoord ).rgb;
	normal_map = (normal_map * 2.0f) - 1.0f;
	mat3 texSpace = mat3(Tan, BiTan, Normal);
	NmNormal = normalize( texSpace * normal_map );

	// Spec data
	vec3 specglow_map = texture( specularTex, TexCoord ).rgb;
	Material.Ks			= specglow_map.x;
	Material.Shininess  = specglow_map.y * 254.0f + 1.0f;
	float glow			= specglow_map.z;

	vec3 ambient = vec3(1.0);
	vec3 diffuse = vec3(0.0);
	vec3 spec    = vec3(0.0);

	ColorData = vec4(ambient + diffuse, 1.0) * albedo_tex + vec4(spec, 0.0f);
}