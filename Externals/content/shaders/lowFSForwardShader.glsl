#version 400
in vec3 Normal;
in vec3 Tan;
in vec3 BiTan;
in vec2 TexCoord;
in vec3 ViewPos;
in vec3 AddColor;

out vec4 ColorData;

//Input textures
uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform sampler2D ShadowDepthTex;

uniform mat4 ViewMatrix;
uniform mat4 BiasMatrix;
uniform mat4 ShadowViewProj;

//Directional light
uniform mediump vec3 dirlightDirection; // Light position in world coords.
uniform mediump vec3 dirlightIntensity; // Diffuse intensity
uniform mediump vec3 dirlightColor;

const int nrOfPointLights = 3;
struct Pointlight {
	vec3 Position; // Light position in world coords.
	vec3 Intensity; // Diffuse intensity
	vec3 Color;
	float Range;
}; 
uniform Pointlight pointlights[nrOfPointLights];

struct MaterialInfo {
	float Ks;
	float Shininess;
};
MaterialInfo Material;

vec3 NmNormal;

void phongModelDirLight(out vec3 ambient, out vec3 diffuse, out vec3 spec) 
{
    ambient = vec3(0.0);
    diffuse = vec3(0.0);
    spec    = vec3(0.0);

    vec3 lightVec = -normalize(( ViewMatrix*vec4(dirlightDirection, 0.0) ).xyz);

	ambient = dirlightColor * dirlightIntensity.x;

	vec3 E = normalize(ViewPos);

	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0)
	{
		// For shadows
		vec4 worldPos = inverse(ViewMatrix) * vec4(ViewPos, 1.0);
		vec4 shadowCoord = BiasMatrix * ShadowViewProj * worldPos;
		
		float shadow = 1.0;
		vec4 shadowCoordinateWdivide = shadowCoord / shadowCoord.w;
		shadowCoordinateWdivide.z -= 0.0005;
		float distanceFromLight = texture(ShadowDepthTex, shadowCoordinateWdivide.st).x;
		
		if (shadowCoord.w > 0.0)
	 		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.0 : 1.0 ;

		// diffuse
		diffuse = diffuseFactor * dirlightColor * dirlightIntensity.y * shadow;

		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * dirlightColor * dirlightIntensity.z * Material.Ks * shadow;        
	}

	return;
}

void phongModel(int index, out vec3 ambient, out vec3 diffuse, out vec3 spec) {

    ambient = vec3(0.0);
	diffuse = vec3(0.0);
	spec    = vec3(0.0);

	vec3 lightVec = (ViewMatrix * vec4(pointlights[index].Position, 1.0)).xyz - ViewPos;
	float d = length(lightVec);

	if(d > pointlights[index].Range)
	return;
	lightVec /= d; //normalizing
        
	ambient = pointlights[index].Color * pointlights[index].Intensity.x;
	vec3 E = normalize(ViewPos);
	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0)
	{
		// diffuse
		diffuse = diffuseFactor * pointlights[index].Color * pointlights[index].Intensity.y;
		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * pointlights[index].Color * pointlights[index].Intensity.z * Material.Ks;          
	}

	float att = 1 - pow((d/pointlights[index].Range), 1.0f);

	ambient *= att;
	diffuse *= att;
	spec    *= att;

	return;
}

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
	vec4 specglow_map = texture( specularTex, TexCoord );
	float blendFactor = specglow_map.w;

	if( AddColor != vec3(0.0) )
		albedo_tex.xyz = (1.0f-blendFactor)*albedo_tex.xyz + blendFactor * AddColor; 

	Material.Ks			= specglow_map.x;
	Material.Shininess  = specglow_map.y * 254.0f + 1.0f;
	float glow			= specglow_map.z;

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 spec    = vec3(0.0);

	if(length( dirlightIntensity ) > 0.0)
	{
		vec3 a,d,s;

		phongModelDirLight(a, d, s);
		ambient += a;
		diffuse += d;
		spec    += s;
	}

	//för varje ljus-----------
	for(int i = 0; i < pointlights.length(); i++)
	{
		vec3 a,d,s;

		if( length(pointlights[i].Intensity) > 0.0 )
		{
			phongModel(i, a, d, s);
			ambient += a;
			diffuse += d;
			spec    += s;
		}
	}

	vec4 glowvec = vec4(glow*albedo_tex.xyz, 1.0);

	ColorData = vec4(ambient + diffuse*(1.0-glow), 1.0) * albedo_tex + vec4(spec, 0.0f) + glowvec;
}