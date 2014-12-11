#version 430 core

layout (local_size_x = 16, local_size_y = 16) in;

//input textures (G-buffer textures)
layout (rgba32f, binding = 0) uniform readonly image2D NormalTex;
layout (rgba8, binding = 1) uniform readonly image2D ColorTex;

// random vector
layout (rgba8, binding = 2) uniform readonly image2D random_vector;

// output 
layout (rgba32f, binding = 5) uniform image2D output_image;

uniform sampler2D DepthTex;

const int nrOfLights = 1;

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

uniform mat4 ViewMatrix;
uniform mat4 invProjection;

ivec2 texelCoord;
vec3 viewPos;
vec3 normal_tex;
vec3 albedo_tex;
float depthVal_tex;

vec2 getRandom()
{
	return normalize( imageLoad(random_vector, ivec2(gl_LocalInvocationID.xy)).xy * 2.0f - 1.0f );
}
/*
float doAmbientOcclusion( vec2 offset )
{
	float g_scale = 8;
	float g_intensity = 12;
	float g_bias = 0.00;

	texelCoord;

	vec3 diff = getPosition(texelCoord + offset) - p;
	vec3 v = normalize(diff);
	float d = length(diff)*g_scale;
	return max(0.0,dot(cnorm,v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}

float ComputeSSAO()
{
	float g_sample_rad = 1.2;
	vec2 vec[4] = { vec2(1,0), vec2(-1,0), vec2(0,1), vec2(0,-1) };

	vec2 rand = getRandom();

	float ao = 0.0f;
	float rad = g_sample_rad/viewPos.z;

	//SSAO Calculation
	for (int j = 0; j < 4; ++j)
	{
		vec2 coord1 = reflect(vec[j],rand)*rad;
		vec2 coord2 = vec2(coord1.x*0.707 - coord1.y*0.707, coord1.x*0.707 + coord1.y*0.707);
  
		ao += doAmbientOcclusion(uv,coord1*0.25, p, n);
		ao += doAmbientOcclusion(uv,coord2*0.5, p, n);
		ao += doAmbientOcclusion(uv,coord1*0.75, p, n);
		ao += doAmbientOcclusion(uv,coord2, p, n);
	} 

	ao/= 16;

	return 1-ao;
}
*/
void phongModel(int index, out vec3 ambient, out vec3 diffuse, out vec3 spec) 
{
    ambient = vec3(0.0);
    diffuse = vec3(0.0);
    spec    = vec3(0.0);

	vec3 thisLightPosition	= vec3(pointlights[index].Position.x, pointlights[index].Position.y, pointlights[index].Position.z);
	vec3 thisLightColor		= vec3(pointlights[index].Color.x, pointlights[index].Color.y, pointlights[index].Color.z);
	vec3 thisLightIntensity = vec3(pointlights[index].Intensity.x, pointlights[index].Intensity.y, pointlights[index].Intensity.z);

    vec3 lightVec = (ViewMatrix * vec4(thisLightPosition, 1.0)).xyz - viewPos;

    float d = length(lightVec);

    if(d > pointlights[index].Range)
        return;
    lightVec /= d; //normalizing
        

	ambient = thisLightColor * thisLightIntensity.x;

	vec3 E = normalize(viewPos);

	float diffuseFactor = dot( lightVec, normal_tex );

	if(diffuseFactor > 0)
	{
		// diffuse
		diffuse = diffuseFactor * thisLightColor * thisLightIntensity;

		// specular
		//vec3 v = normalize(2 * Material.Ks * normal_tex - lightVec);//reflect( lightVec, normal_tex );
		vec3 v = reflect( lightVec, normal_tex );
		//float specFactor = max( pow( max( dot(v, E), 0.0f ), Material.Shininess ), 0.0f);
		float specFactor = pow( max( dot(v, E), 0.0 ), Material.Shininess );
		spec = specFactor * thisLightColor * thisLightIntensity.z * Material.Ks;        
	}

	float att = 1 - pow((d/pointlights[index].Range), 1.0f);

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
	//pointlights[0].Position = vec4(0.0, 3.0, 0.0, 1.0);
	//pointlights[0].Intensity = vec3(0.5, 0.9, 0.9);
	//pointlights[0].Color = vec3(0.9);
	//pointlights[0].Range = 10.0f;

	texelCoord = ivec2( gl_GlobalInvocationID.x, gl_GlobalInvocationID.y);

	// Retrieve position, normal and color information from the g-buffer textures
	vec4 inputMap0 = texelFetch(DepthTex, texelCoord, 0);
	vec4 inputMap1 = imageLoad(NormalTex, texelCoord);
	vec4 inputMap2 = imageLoad(ColorTex, texelCoord);

	// Normal haxxx
	normal_tex.xy = inputMap1.xy;
	normal_tex.z = sqrt( 1 - (normal_tex.x*normal_tex.x) - (normal_tex.y*normal_tex.y) );
	//--------------
	Material.Ks = inputMap1.z;
    Material.Shininess = inputMap1.w * 254.0f + 1.0f;
	//--------------
	albedo_tex.xyz = inputMap2.xyz;
	//--------------
	float glow = inputMap2.w;
	//--------------
	depthVal_tex = inputMap0.x;

	vec2 ndc = vec2(texelCoord) / vec2( gl_WorkGroupSize.xy*gl_NumWorkGroups.xy ) * 2.0f - 1.0f;
	viewPos = reconstructPosition(depthVal_tex, ndc);

	vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 spec    = vec3(0.0);

	//för varje ljus-----------
	for(int i = 0; i < pointlights.length(); i++)
	{
		vec3 a,d,s;

		phongModel(i, a, d, s);
		ambient += a;
		diffuse += d;
		spec    += s;
	}
	//-------------------------

	//DIR LIGHT ---------------
	/*vec3 lightDir = (ViewMatrix * vec4(-3, 4, -2, 0)).xyz;
	float lightIntensity = max(dot(normalize(lightDir), normalize(normal_tex)), 0.0 );;
	if (lightIntensity > 0.0f)
		diffuse += (vec3(0.6, 0.6, 0.6) * lightIntensity);*/
	//-------------------------

	vec4 FragColor = vec4(ambient + diffuse, 1.0) * vec4(albedo_tex, 1.0) + vec4(spec, 0.0f);
	//FragColor = vec4(diffuse, 1.0);
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