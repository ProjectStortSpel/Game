#version 100
#extension GL_OES_fragment_precision_high : enable

precision highp float;

varying vec3 Normal;
varying vec3 Tan;
varying vec3 BiTan;
varying vec2 TexCoord;
varying vec4 ViewPos;

//Input textures
uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform mat4 ViewMatrix;
uniform mat4 BiasMatrix;

uniform vec3 BlendColor;

//Directional light
uniform mediump vec3 dirlightDirection; // Light position in world coords.
uniform mediump vec3 dirlightIntensity; // Diffuse intensity
uniform mediump vec3 dirlightColor;

struct Pointlight {
	vec3 Position; // Light position in world coords.
	vec3 Intensity; // Diffuse intensity
	vec3 Color;
	float Range;
}; 
uniform Pointlight pointlights[2];

struct MaterialInfo {
	float Ks;
	float Shininess;
}; MaterialInfo Material;

uniform float ElapsedTime;

vec3 NmNormal;

void phongModelDirLight(out vec3 ambient, out vec3 diffuse, out vec3 spec) 
{
    ambient = vec3(0.0);
    diffuse = vec3(0.0);
    spec    = vec3(0.0);

    vec3 lightVec = -normalize(( ViewMatrix*vec4(dirlightDirection, 0.0) ).xyz);

	ambient = dirlightColor * dirlightIntensity.x;

	vec3 E = normalize(ViewPos.xyz);

	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0.0)
	{
		// diffuse
		diffuse = diffuseFactor * dirlightColor * dirlightIntensity.y;

		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * dirlightColor * dirlightIntensity.z * Material.Ks;        
	}

	return;
}

void phongModel(Pointlight pointlight, out vec3 ambient, out vec3 diffuse, out vec3 spec) 
{
	ambient = vec3(0.0);
	diffuse = vec3(0.0);
	spec    = vec3(0.0);

	vec3 lightVec = (ViewMatrix * vec4(pointlight.Position, 1.0)).xyz - ViewPos.xyz;
	float d = length(lightVec);

	if(d > pointlight.Range)
	  return;
	lightVec /= d; //normalizing
        
	ambient = pointlight.Color * pointlight.Intensity.x;
	vec3 E = normalize(ViewPos.xyz);
	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0.0)
	{
		// diffuse
		diffuse = diffuseFactor * pointlight.Color * pointlight.Intensity.y;
		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * pointlight.Color * pointlight.Intensity.z * Material.Ks;          
	}

	float att = 1.0 - pow((d/pointlight.Range), 1.0);

	ambient *= att;
	diffuse *= att;
	spec    *= att;

	return;
}


void main() 
{
	float v = atan(TexCoord.y / TexCoord.x);

	vec2 finalCoord;

	finalCoord.x = TexCoord.x / cos(v);
	finalCoord.x = finalCoord.x / sqrt(2.0);

	float HalfPI = 1.570796327;
	v = v + ElapsedTime * (HalfPI / 2.5); // -
	v = v - floor(v/HalfPI) * HalfPI;

	finalCoord.y = v / HalfPI;
	//----------------------------------------------

	vec4 albedo_tex = texture2D( diffuseTex, finalCoord );

	// Normal data
	vec3 normal_map	  = texture2D( normalTex, finalCoord ).rgb;
	normal_map = (normal_map * 2.0) - 1.0;
	mat3 texSpace = mat3(Tan, BiTan, Normal);
	NmNormal = normalize( texSpace * normal_map );

	// Spec data
	vec4 spec_map = texture2D( specularTex, finalCoord );

	float mMod = spec_map.a*99.0 - 50.0 * floor((spec_map.a*99.0)/50.0);
	float blendFactor = mMod/50.0;//(specTexture.a-0.5f)*2;
	vec3 AddedColor = BlendColor;
	if (spec_map.a < 0.5)
		AddedColor = vec3(1) - BlendColor; // ANTICOLOR? Good or bad? I like

	if( BlendColor != vec3(0.0) )
		albedo_tex.xyz = (1.0-blendFactor)*albedo_tex.xyz + blendFactor * AddedColor; 

	Material.Ks			= spec_map.x;
	Material.Shininess  = spec_map.y * 254.0 + 1.0;
    
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 spec 	 = vec3(0.0);
    
	if(length( dirlightIntensity ) > 0.0)
	{
		vec3 a,d,s;

		phongModelDirLight(a, d, s);
		ambient += a;
		diffuse += d;
		spec    += s;
	}

    vec3 a,d,s;
    //för varje ljus-----------
	if( length(pointlights[0].Intensity) > 0.0)
	{
		phongModel(pointlights[0], a, d, s);
		diffuse += d; ambient += a; spec += s;
	}
	if( length(pointlights[1].Intensity) > 0.0)
	{
		phongModel(pointlights[1], a, d, s);
		diffuse += d; ambient += a; spec += s;
	}

	float glow = spec_map.z;
	vec4 glowvec = vec4(glow*albedo_tex.xyz, 0.0);
    
    gl_FragColor = vec4(ambient + diffuse*(1.0-glow), 1.0) * albedo_tex + vec4(spec, 0.0) + glowvec;
	//gl_FragColor = vec4( (inverse(ViewMatrix) * vec4(Normal, 0.0)).xyz, 1.0);
	//gl_FragColor = albedo_tex; //vec4(Normal, 1.0);
    //gl_FragColor = vec4(ambient + diffuse, 1.0) * vec4(0.0, 0.0, 1.0, 1.0) + vec4(spec, 0.0); //albedo_tex;
}




