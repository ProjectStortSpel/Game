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

uniform mat4 ViewMatrix;

const int nrOfLights = 1;
struct LightInfo {
	vec4 Position; // Light position in world coords.
	vec3 Intensity; // Diffuse intensity
	vec3 Color;
	float Range;
};
LightInfo Lights[nrOfLights];

struct MaterialInfo {
	float Ks;
	float Shininess;
};
MaterialInfo Material;

vec3 NmNormal;

void phongModel(int index, out vec3 ambient, out vec3 diffuse, out vec3 spec) {

    ambient = vec3(0.0);
	diffuse = vec3(0.0);
	spec    = vec3(0.0);

	vec3 lightVec = (ViewMatrix * Lights[index].Position).xyz - ViewPos;
	float d = length(lightVec);

	if(d > Lights[index].Range)
	return;
	lightVec /= d; //normalizing
        
	ambient = Lights[index].Color * Lights[index].Intensity.x;
	vec3 E = normalize(ViewPos);
	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0)
	{
		// diffuse
		diffuse = diffuseFactor * Lights[index].Color * Lights[index].Intensity.y;
		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * Lights[index].Color * Lights[index].Intensity.z * Material.Ks;        
	}

	float att = 1 - pow((d/Lights[index].Range), 1.0f);

	ambient *= att;
	diffuse *= att;
	spec    *= att;

	return;
}

void main() 
{
	Lights[0].Position = vec4(0.0, 1.0, 3.0, 1.0);
	Lights[0].Intensity = vec3(0.2, 0.9, 0.9);
	Lights[0].Color = vec3(0.9);
	Lights[0].Range = 10.0f;

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


	vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 spec    = vec3(0.0);

	//för varje ljus-----------
	for(int i = 0; i < nrOfLights; i++)
	{
		vec3 a,d,s;

		phongModel(i, a, d, s);
		ambient += a;
		diffuse += d;
		spec    += s;
	}

	vec4 albedo_tex = texture( diffuseTex, TexCoord );

	ColorData = vec4(ambient + diffuse, 1.0) * albedo_tex + vec4(spec, 0.0f);
}