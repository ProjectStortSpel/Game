#version 430 core

layout (local_size_x = 16, local_size_y = 16) in;

//input textures (G-buffer textures)
layout (rgba32f, binding = 0) uniform readonly image2D NormalTex;
layout (rgba8, binding = 1) uniform readonly image2D ColorTex;

// output 
layout (rgba32f, binding = 5) uniform image2D output_image;

uniform sampler2D DepthTex;

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
	float Ks;
	float Shininess;
};
//uniform 
MaterialInfo Material;

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

        lightVec /= d; //normalizing
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

	ivec2 texelCoord = ivec2( gl_GlobalInvocationID.x, gl_GlobalInvocationID.y);

	// Retrieve position, normal and color information from the g-buffer textures
	// Normal haxxx
	normal_tex.xy = vec2( imageLoad(NormalTex, texelCoord) );
	normal_tex.z = sqrt( 1 - (normal_tex.x*normal_tex.x) - (normal_tex.y*normal_tex.y) );
	//--------------
	albedo_tex.xyz = vec3( imageLoad(ColorTex, texelCoord) );
	depthVal_tex = texelFetch(DepthTex, texelCoord, 0).x;

	vec2 ndc = vec2(texelCoord) / vec2( gl_WorkGroupSize.xy*gl_NumWorkGroups.xy ) * 2.0f - 1.0f;
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

	vec4 FragColor = vec4(ambient + diffuse, 1.0) * vec4(albedo_tex, 1.0) + vec4(spec, 0.0f);
	//vec4 FragColor = vec4( normal_tex, 1.0);
	//vec4 FragColor = vec4( albedo_tex   +normal_tex-normal_tex, 1.0 );
	//vec4 FragColor = vec4( vec3(viewPos  +normal_tex-normal_tex), 1.0 );
	//vec4 FragColor = vec4( vec3(depthVal_tex), 1.0 );

	imageStore(
		output_image,
		ivec2(gl_GlobalInvocationID.xy),
		FragColor
	);
}