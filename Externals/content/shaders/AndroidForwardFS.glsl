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

uniform mat4 ViewMatrix;

struct DirectionalLight {
	vec3 Direction; // Light position in world coords.
	vec3 Intensity; // Diffuse intensity
	vec3 Color;
};
uniform DirectionalLight dirlight;


struct Pointlight {
	vec3 Position; // Light position in world coords.
	vec3 Intensity; // Diffuse intensity
	vec3 Color;
	float Range;
};
Pointlight pointlights[1];

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

    vec3 lightVec = -normalize(( ViewMatrix*vec4(dirlight.Direction, 0.0) ).xyz);

	ambient = dirlight.Color * dirlight.Intensity.x;

	vec3 E = normalize(ViewPos);

	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0.0)
	{
		// diffuse
		diffuse = diffuseFactor * dirlight.Color * dirlight.Intensity.y;

		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * dirlight.Color * dirlight.Intensity.z * Material.Ks;        
	}

	return;
}

void phongModel(int index, out vec3 ambient, out vec3 diffuse, out vec3 spec) {

	ambient = vec3(0.0);
	diffuse = vec3(0.0);
	spec    = vec3(0.0);

	vec3 thisLightPosition	= pointlights[0].Position;
	vec3 thisLightColor	= pointlights[0].Color;
	vec3 thisLightIntensity = pointlights[0].Intensity;

	vec3 lightVec = (ViewMatrix * vec4(thisLightPosition, 1.0)).xyz - ViewPos;
	float d = length(lightVec);

	if(d > pointlights[0].Range)
	  return;
	lightVec /= d; //normalizing
        
	ambient = thisLightColor * thisLightIntensity.x;
	vec3 E = normalize(ViewPos);
	float diffuseFactor = dot( lightVec, NmNormal );

	if(diffuseFactor > 0.0)
	{
		// diffuse
		diffuse = diffuseFactor * thisLightColor * thisLightIntensity.y;
		// specular
		vec3 v = reflect( lightVec, NmNormal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * thisLightColor * thisLightIntensity.z * Material.Ks;          
	}

	float att = 1.0 - pow((d/pointlights[0].Range), 1.0);

	ambient *= att;
	diffuse *= att;
	spec    *= att;

	return;
}


void main() 
{
    pointlights[0].Position = vec3(8.0, 10.0, 8.0);
    pointlights[0].Intensity = vec3(0.2, 1.0, 1.0);
    pointlights[0].Color = vec3(1.0, 1.0, 1.0);
    pointlights[0].Range = 20.0;

	vec4 albedo_tex = texture2D( diffuseTex, TexCoord );

	// Normal data
	vec3 normal_map	  = texture2D( normalTex, TexCoord ).rgb;
	normal_map = (normal_map * 2.0) - 1.0;
	mat3 texSpace = mat3(Tan, BiTan, Normal);
	NmNormal = normalize( texSpace * normal_map );

	// Spec data
	vec3 spec_map = texture2D( specularTex, TexCoord ).rgb;
	Material.Ks			= spec_map.x;
	Material.Shininess  = spec_map.y * 254.0 + 1.0;
    
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 spec 	 = vec3(0.0);
    
	if(length( dirlight.Intensity ) > 0.0)
	{
		vec3 a,d,s;

		phongModelDirLight(a, d, s);
		ambient += a;
		diffuse += d;
		spec    += s;
	}

    //för varje ljus-----------
    //for(int i = 0; i < 1; i++)
    //{
	    //vec3 a,d,s;

	    //phongModel(0, a, d, s);
	    //diffuse += d;
	    //ambient += a;
	    //spec    += s;
    //}
    
    gl_FragColor = vec4(ambient + diffuse, 1.0) * albedo_tex + vec4(spec, 0.0);
	//gl_FragColor = vec4( (inverse(ViewMatrix) * vec4(Normal, 0.0)).xyz, 1.0);
	//gl_FragColor = vec4(Normal, 1.0);
    //gl_FragColor = vec4(ambient + diffuse, 1.0) * vec4(0.0, 0.0, 1.0, 1.0) + vec4(spec, 0.0); //albedo_tex;
}




