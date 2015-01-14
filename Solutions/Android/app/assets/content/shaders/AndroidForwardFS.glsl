varying vec3 Normal;
//varying vec3 Tan;
//varying vec3 BiTan;
//varying vec2 TexCoord;
varying vec3 ViewPos;


//Input textures
//uniform sampler2D diffuseTex;
//uniform sampler2D normalTex;
//uniform sampler2D specularTex;

uniform mat4 ViewMatrix;

//struct DirectionalLight {
//	vec3 Direction; // Light position in world coords.
//	vec3 Intensity; // Diffuse intensity
//	vec3 Color;
//};

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

void phongModel(int index, out vec3 ambient, out vec3 diffuse, out vec3 spec) {

	ambient = vec3(0.0);
	diffuse = vec3(0.0);
	spec    = vec3(0.0);

	vec3 thisLightPosition	= pointlights[index].Position;
	vec3 thisLightColor	= pointlights[index].Color;
	vec3 thisLightIntensity = pointlights[index].Intensity;

	vec3 lightVec = (ViewMatrix * vec4(thisLightPosition, 1.0)).xyz - ViewPos;
	float d = length(lightVec);

	if(d > pointlights[index].Range)
	return;
	lightVec /= d; //normalizing
        
	ambient = thisLightColor * thisLightIntensity.x;
	vec3 E = normalize(ViewPos);
	float diffuseFactor = dot( lightVec, Normal );

	if(diffuseFactor > 0.0)
	{
		// diffuse
		diffuse = diffuseFactor * thisLightColor * thisLightIntensity.y;
		// specular
		vec3 v = reflect( lightVec, Normal );
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * thisLightColor * thisLightIntensity.z * Material.Ks;          
	}

	float att = 1.0 - pow((d/pointlights[index].Range), 1.0f);

	ambient *= att;
	diffuse *= att;
	spec    *= att;

	return;
}


void main() 
{
    pointlights[0].Position = vec3(8.0f, 10.0f, 8.0f);
    pointlights[0].Intensity = vec3(0.2f, 1.0f, 1.0f);
    pointlights[0].Color = vec3(1.0f, 1.0f, 1.0f);
    pointlights[0].Range = 20.0f;
    
    Material.Ks = 0.1f;
    Material.Shininess = 20.0f;
    
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 spec 	 = vec3(0.0f);
    
    //för varje ljus-----------
    for(int i = 0; i < 1; i++)
    {
	    vec3 a,d,s;

	    phongModel(i, a, d, s);
	    ambient += a;
	    diffuse += d;
	    spec    += s;
    }
    
    //vec4 albedo_tex = texture2D( diffuseTex, TexCoord );

    //gl_FragColor = vec4(ambient + diffuse, 1.0) * albedo_tex + vec4(spec, 0.0f);
    gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);// * vec4(ambient + diffuse, 1.0f) * vec4(spec, 1.0f); //albedo_tex;
}




