#version 440
in vec3 Normal;
in vec3 Tan;
in vec3 BiTan;
in vec2 TexCoord;
in vec3 ViewPos;
in vec3 AddColor;
in float A;

layout( location = 0 ) out vec4 ColorData;

//Input textures
uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform sampler2D ShadowDepthTex;

uniform mat4 ViewMatrix;
uniform mat4 BiasMatrix;
uniform mat4 ShadowViewProj;

struct vector3
{
	float x, y, z;
};

struct DirectionalLight {
	vector3 Direction; // Light position in world coords.
	vector3 Intensity; // Diffuse intensity
	vector3 Color;
};

struct Pointlight {
	vector3 Position; // Light position in world coords.
	vector3 Intensity; // Diffuse intensity
	vector3 Color;
	float Range;
};

layout (std430, binding = 3) buffer DirectionalLights   
{
	DirectionalLight directionalLight;
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

void phongModelDirLight(out vec3 ambient, out vec3 diffuse, out vec3 spec) 
{
    ambient = vec3(0.0);
    diffuse = vec3(0.0);
    spec    = vec3(0.0);

	vec3 thisLightDirection	= vec3(directionalLight.Direction.x,	directionalLight.Direction.y,	directionalLight.Direction.z);
	vec3 thisLightColor		= vec3(directionalLight.Color.x,		directionalLight.Color.y,		directionalLight.Color.z);
	vec3 thisLightIntensity = vec3(directionalLight.Intensity.x,	directionalLight.Intensity.y,	directionalLight.Intensity.z);

    vec3 lightVec = -normalize(( ViewMatrix*vec4(thisLightDirection, 0.0) ).xyz);

	ambient = thisLightColor * thisLightIntensity.x;

	vec3 E = normalize(ViewPos);

	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0)
	{
		// For shadows
		vec4 worldPos = inverse(ViewMatrix) * vec4(ViewPos, 1.0);
		vec4 shadowCoord = BiasMatrix * ShadowViewProj * worldPos;
		// = textureProj(ShadowDepthTex, shadowCoord);
		// The sum of the comparisons with nearby texels  
		//float sum = 0;
		//// Sum contributions from texels around ShadowCoord  
		//sum += textureProjOffset(ShadowDepthTex, shadowCoord, ivec2(-1,-1));  
		//sum += textureProjOffset(ShadowDepthTex, shadowCoord, ivec2(-1,1));  
		//sum += textureProjOffset(ShadowDepthTex, shadowCoord, ivec2(1,1));  
		//sum += textureProjOffset(ShadowDepthTex, shadowCoord, ivec2(1,-1));  

		//float shadowResult = sum * 0.25;
		////float shadowResult = textureProj(ShadowDepthTex, shadowCoord);
		////float bias = 0.23; //0.00022 + (shadowCoord.z*shadowCoord.z)*0.5; //= 0.022;
		//float bias = min( pow(shadowCoord.z, 10 - 10*shadowCoord.z), 0.40);

		//if ( shadowResult < (shadowCoord.z - bias)/shadowCoord.w) 
		//{
		//   shadow = shadowResult;
		//}
		
		float shadow = 1.0;
		vec4 shadowCoordinateWdivide = shadowCoord / shadowCoord.w;
		shadowCoordinateWdivide.z -= 0.0005;
		float distanceFromLight = texture(ShadowDepthTex, shadowCoordinateWdivide.st).z;
		
		if (shadowCoord.w > 0.0)
	 		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.0 : 1.0 ;

		// diffuse
		diffuse = diffuseFactor * thisLightColor * thisLightIntensity.y * shadow;

		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * thisLightColor * thisLightIntensity.z * Material.Ks * shadow;        
	}

	return;
}

void phongModel(int index, out vec3 ambient, out vec3 diffuse, out vec3 spec) {

    ambient = vec3(0.0);
	diffuse = vec3(0.0);
	spec    = vec3(0.0);

	vec3 thisLightPosition	= vec3(pointlights[index].Position.x, pointlights[index].Position.y, pointlights[index].Position.z);
	vec3 thisLightColor		= vec3(pointlights[index].Color.x, pointlights[index].Color.y, pointlights[index].Color.z);
	vec3 thisLightIntensity = vec3(pointlights[index].Intensity.x, pointlights[index].Intensity.y, pointlights[index].Intensity.z);

	vec3 lightVec = (ViewMatrix * vec4(thisLightPosition, 1.0)).xyz - ViewPos;
	float d = length(lightVec);

	if(d > pointlights[index].Range)
	return;
	lightVec /= d; //normalizing
        
	ambient = thisLightColor * thisLightIntensity.x;
	vec3 E = normalize(ViewPos);
	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0)
	{
		// diffuse
		diffuse = diffuseFactor * thisLightColor * thisLightIntensity.y;
		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * thisLightColor * thisLightIntensity.z * Material.Ks;          
	}

	float att = 1 - pow((d/pointlights[index].Range), 1.0f);

	ambient *= att;
	diffuse *= att;
	spec    *= att;

	return;
}

uniform float ElapsedTime;

void main() 
{
	float v = atan(TexCoord.y / TexCoord.x);

	vec2 finalCoord;

	finalCoord.x = TexCoord.x / cos(v);
	finalCoord.x = finalCoord.x / sqrt(2);

	finalCoord.y = TexCoord.y - 0.1f * ElapsedTime;

	if( finalCoord.y < 0.0f )
	{
		finalCoord.y = finalCoord.y - (floor(finalCoord.y)-1);
	}

	v = atan(finalCoord.y / TexCoord.x);
	finalCoord.y = sin(v);

	//vec2 modifiedCoord = TexCoord;
	//vec2 cornerCoord;
	//if(A < 0.0)
	//	cornerCoord = vec2(0.0, 0.0);
	//else if(A > 0.0)
	//	cornerCoord = vec2(0.0, 1.0);

	//float dY = TexCoord.y - cornerCoord.y;

	//TexCoord.y = cornerCoord.y + TexCoord.x * dY;
	
	//----------------------------------------------

	// Diffuse tex
	vec4 albedo_tex = texture( diffuseTex, finalCoord );

	// Normal data
	vec3 normal_map	  = texture( normalTex, finalCoord ).rgb;
	normal_map = (normal_map * 2.0f) - 1.0f;
	mat3 texSpace = mat3(Tan, BiTan, Normal);
	NmNormal = normalize( texSpace * normal_map );

	// Spec data
	vec4 specglow_map = texture( specularTex, finalCoord );
	float blendFactor = specglow_map.w;

	if( AddColor != vec3(0.0) )
		albedo_tex.xyz = (1.0f-blendFactor)*albedo_tex.xyz + blendFactor * AddColor; 

	Material.Ks			= specglow_map.x;
	Material.Shininess  = specglow_map.y * 254.0f + 1.0f;
	float glow			= specglow_map.z;

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 spec    = vec3(0.0);

	if(length( vec3(directionalLight.Intensity.x, directionalLight.Intensity.y, directionalLight.Intensity.z) ) > 0.0)
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

		phongModel(i, a, d, s);
		ambient += a;
		diffuse += d;
		spec    += s;
	}

	vec4 glowvec = vec4(glow*albedo_tex.xyz, 0.0);

	ColorData = vec4(ambient + diffuse*(1.0-glow), 1.0) * albedo_tex + vec4(spec, 0.0f) + glowvec;
}