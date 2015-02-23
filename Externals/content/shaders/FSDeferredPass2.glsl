#version 430
in vec2 TexCoord;
out vec4 FragColor;

const int nrOfLights = 1;
struct LightInfo {
	vec4 Position; // Light position in world coords.
	vec3 Intensity; // Diffuse intensity
	vec3 Color;
	float Range;
};
//uniform 
LightInfo Lights[nrOfLights];

struct MaterialInfo {
	//vec3 Kd; // Diffuse reflectivity
	float Ks;
	float Shininess;
};
//uniform 
MaterialInfo Material;

// The g-buffer textures
uniform sampler2D NormalTex;
uniform sampler2D ColorTex;

uniform sampler2D DepthTex;

uniform mat4 ViewMatrix;
uniform mat4 invProjection;

vec3 viewPos;
vec3 normal_tex;
vec3 albedo_tex;
float depthVal_tex;

void phongModel(int index, out vec3 ambient, out vec3 diffuse, out vec3 spec) {

        //tmp material
        Material.Ks = 0.7;
        Material.Shininess = 30.0;

        ambient = vec3(0.0);
        diffuse = vec3(0.0);
        spec    = vec3(0.0);

        vec3 lightVec = (ViewMatrix * Lights[index].Position).xyz - viewPos;

        float d = length(lightVec);

        if(d > Lights[index].Range)
            return;

	   ambient = Lights[index].Color * Lights[index].Intensity.x;

        lightVec /= d; //normlizing
        float diffuseFactor = dot( lightVec, normal_tex );

        if(diffuseFactor > 0)
        {
            vec3 v = reflect( lightVec, normal_tex );
            float specFactor = pow( max( dot(v, normalize(viewPos)), 0.0 ), Material.Shininess );

            diffuse = diffuseFactor * Lights[index].Color * Lights[index].Intensity.y;
            spec = specFactor * Lights[index].Color * Lights[index].Intensity.z * Material.Ks;
        }

        float att = 1 - pow((d/Lights[index].Range), 1.0f);

        ambient *= att;
        diffuse *= att;
        spec    *= att;

        return;
}

vec3 reconstructPosition(float p_depth, vec2 p_ndc)
{ 
    vec4 sPos = vec4(p_ndc, p_depth*2.0f - 1.0f, 1.0);
    sPos = invProjection * sPos;

    return (sPos.xyz / sPos.w);
}

void main() 
{
	Lights[0].Position = vec4(0.0, 0.0, 3.0, 1.0);
	Lights[0].Intensity = vec3(0.2, 0.9, 0.9);
	Lights[0].Color = vec3(0.9);
	Lights[0].Range = 10.0f;

	vec2 uv = (TexCoord + vec2(1.0f)) / 2.0f;
    //uv.y = 1.0f - uv.y;

	// Retrieve position, normal and color information from the g-buffer textures
	// Normal haxxx
	normal_tex.xy = vec2( texture( NormalTex, uv ) );
	normal_tex.z = sqrt( 1 - (normal_tex.x*normal_tex.x) - (normal_tex.y*normal_tex.y) );
	//--------------
	albedo_tex = vec3( texture(ColorTex, uv) );
	depthVal_tex = texture(DepthTex, uv).x;

	vec2 ndc = uv * 2.0f - 1.0f;
	viewPos = reconstructPosition(depthVal_tex, ndc);

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
	//-------------------------

	FragColor = vec4(ambient + diffuse, 1.0) * vec4(albedo_tex, 1.0) + vec4(spec, 0.0f);
	//FragColor = vec4( normal_tex, 1.0);
	//FragColor = vec4( albedo_tex, 1.0 );
	//FragColor = vec4( vec3(viewPos), 1.0 );
}

