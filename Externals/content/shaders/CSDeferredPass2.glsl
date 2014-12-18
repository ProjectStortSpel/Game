#version 430 core
layout (local_size_x = 16, local_size_y = 16) in;

// ---- Structs ---- 
struct vector3 { float x, y, z; };
struct DirectionalLight { vector3 Direction; vector3 Intensity; vector3 Color; };
struct Pointlight {	vector3 Position; vector3 Intensity; vector3 Color; float Range; };

// ---- INPUTS ----
uniform sampler2D DepthTex;
uniform sampler2DShadow ShadowDepthTex;
layout (rgba32f, binding = 0) uniform readonly image2D NormalTex;
layout (rgba8, binding = 1) uniform readonly image2D ColorTex;
layout (rgba8, binding = 2) uniform readonly image2D RandomTex;
layout (std430, binding = 3) buffer DirectionalLights { DirectionalLight directionalLight; };
layout (std430, binding = 4) buffer PointLights { Pointlight pointlights[]; };

// ---- OUTPUT ---- 
layout (rgba32f, binding = 5) uniform image2D output_image;

// ---- Uniforms ---- 
uniform mat4 ViewMatrix;
uniform mat4 InvProjection;
uniform mat4 BiasMatrix;
uniform mat4 ShadowViewProj;

// ---- PER THREAD GLOBALS ---- 
ivec2 g_threadID;		// sample pixel position
vec3 g_viewPos;			// position in view space
vec3 g_albedo;			// albedo color (ColorTex.xyz)
vec3 g_normal;			// normal (NormalTex.xy, haxx)
float g_ks;				// spec (NormalTex.z)
float g_shininess;		// spec (NormalTex.w)
float g_depthVal;		// depthval (DepthTex.x)

// ---- FUNCTION DEFINES ---- 
vec2 getRandom();
vec3 getPosition(ivec2 samplePos);
float doAmbientOcclusion( vec2 offset );
float ComputeSSAO();
float ComputeGlow();

void ComputeDirLight(out vec3 ambient, out vec3 diffuse, out vec3 spec);

void phongModelDirLight(out vec3 ambient, out vec3 diffuse, out vec3 spec);
void phongModel(int index, out vec3 ambient, out vec3 diffuse, out vec3 spec);
vec3 reconstructPosition(float p_depth);

// ---- MAIN ---- 
void main() 
{
	vec4 FragColor = vec4(0.0);

	// Get threadID
	g_threadID = ivec2( gl_GlobalInvocationID.x, gl_GlobalInvocationID.y );

	// Retrieve position, normal and color information from the g-buffer textures
	vec4 inputMap0 = texelFetch(DepthTex, g_threadID, 0);
	vec4 inputMap1 = imageLoad(NormalTex, g_threadID);
	vec4 inputMap2 = imageLoad(ColorTex, g_threadID);

	// Set globals
	g_normal.xy = inputMap1.xy;
	g_normal.z = sqrt( 1 - (g_normal.x*g_normal.x) - (g_normal.y*g_normal.y) );
	g_ks = inputMap1.z;
    g_shininess = inputMap1.w * 254.0f + 1.0f;
	g_albedo.xyz = inputMap2.xyz;
	g_depthVal = inputMap0.x;
	g_viewPos = reconstructPosition(g_depthVal);

	

	// Values
	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 spec    = vec3(0.0);

	if((g_depthVal < 1.0))
	{
		// Calc DirLight
		ComputeDirLight(ambient, diffuse, spec);

		// Calc PointLights
		for(int i = 0; i < pointlights.length(); i++)
		{
			vec3 a,d,s;

			phongModel(i, a, d, s);
			ambient += a;
			diffuse += d;
			spec    += s;
		}
	}

	// Do post renderer effect here
	float glow = ComputeGlow();
	vec4 glowvec = vec4(glow,glow,glow, 1.0);
	float SSAO = ComputeSSAO();
	vec4 SSAOvec = vec4(SSAO,SSAO,SSAO, 1.0);


	// Do frag calcs here
	FragColor = vec4(ambient + diffuse, 1.0) * vec4(g_albedo, 1.0) * SSAOvec + vec4(spec, 0.0f) + glowvec;
	//FragColor = glowvec;
	FragColor = SSAOvec;
	//FragColor = vec4( g_normal, 1.0);
	//FragColor = vec4( g_albedo   +g_normal-g_normal, 1.0 );
	//FragColor = vec4( vec3(g_viewPos  +g_normal-g_normal), 1.0 );
	//FragColor = vec4( vec3(g_depthVal), 1.0 );
	
	imageStore(
		output_image,
		ivec2(gl_GlobalInvocationID.xy),
		FragColor
	);
}



// ---- FUNCTIONS ---- 

float ComputeGlow()
{
	// IMPROVEMENTS CAN BE MADE. RIGHT NOW WE SAMPLE IN A SPIRAL. BETTER SOLUTION WOULD BE TO DO 2 SAMPLES AFTER EACHOTHER. BUT THIS WOULD NEED A NEW TEXTURE MAP AND A BARRIER. AND COST MORE.
	/*
	- - - - - - 5 - - - - - - 
	- - - - - - - - - - 6 - - 
	- - - 4 - - - - - - - - - 
	- - - - - - - - - - - - - 
	- - - - - - 1 2 - - - - - 
	7 - - 3 - - 0 - - 3 - - 7
	- - - - - 2 1 - - - - - - 
	- - - - - - - - - - - - - 
	- - - - - - - - - 4 - - - 
	- - 6 - - - - - - - - - - 
	- - - - - - 5 - - - - - - 
	*/
	float glow = 0;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(-6, 0)).w * 0.0044299121055113265;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(-4, -4)).w * 0.00895781211794;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(0, -5)).w * 0.0215963866053;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(3, -3)).w * 0.0443683338718;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(-3, 0)).w * 0.0776744219933;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(-1, -1)).w * 0.115876621105;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(0, -1)).w * 0.147308056121;
	//glow += imageLoad(ColorTex, g_threadID).w * 0.159576912161;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(0,  1)).w * 0.147308056121;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(1,  1)).w * 0.115876621105;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(3,  0)).w * 0.0776744219933;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(-3,  3)).w * 0.0443683338718;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(0,  5)).w * 0.0215963866053;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(4,  4)).w * 0.00895781211794;
	//glow += imageLoad(ColorTex, g_threadID + ivec2(6,  0)).w * 0.0044299121055113265;
	
	return glow;
}

vec3 getPosition(ivec2 samplePos)
{
	float depthVal = texelFetch(DepthTex, samplePos, 0).x;


	vec2 ndc = vec2(g_threadID) / vec2( gl_WorkGroupSize.xy*gl_NumWorkGroups.xy - ivec2(1,1)) * 2.0f - 1.0f;

    vec4 H = vec4(	
					ndc,
					depthVal * 2.0 - 1.0,
					1.0
				);

    vec4 D = InvProjection * H;
    return (D.xyz / D.w);
}

vec2 getRandom()
{
	return normalize( imageLoad(RandomTex, ivec2(gl_LocalInvocationID.xy)).xy * 2.0f - 1.0f );
}


float doAmbientOcclusion( vec2 offset )
{
	float g_scale = 0.8;
	float g_intensity = 1.2;
	float g_bias = 0.00;

	offset = offset * vec2(gl_WorkGroupSize.xy*gl_NumWorkGroups.xy);

	ivec2 ssaoSamp;
	ssaoSamp.x = int(min(max(g_threadID.x + ivec2(offset).x, 0), gl_WorkGroupSize.x*gl_NumWorkGroups.x-1));
	ssaoSamp.y = int(min(max(g_threadID.y + ivec2(offset).y, 0), gl_WorkGroupSize.y*gl_NumWorkGroups.y-1));

	vec3 diff = getPosition(ssaoSamp) - g_viewPos;
	vec3 v = normalize(diff);
	float d = length(diff)*g_scale;
	return max(0.0,dot(g_normal,v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}

float ComputeSSAO()
{
	float g_sample_rad = 0.12;
	vec2 vec[4] = { vec2(1,0), vec2(-1,0), vec2(0,1), vec2(0,-1) };

	vec2 rand = getRandom();

	float ao = 0.0f;
	float rad = g_sample_rad/g_viewPos.z;

	//SSAO Calculation
	for (int j = 0; j < 4; ++j)
	{
		vec2 coord1 = vec2(reflect(vec[j],rand)*rad);
		vec2 coord2 = vec2(coord1.x*0.707 - coord1.y*0.707, coord1.x*0.707 + coord1.y*0.707);
  
		ao += doAmbientOcclusion(coord1*0.25);
		ao += doAmbientOcclusion(coord2*0.5);
		ao += doAmbientOcclusion(coord1*0.75);
		ao += doAmbientOcclusion(coord2);
	} 

	ao/= 16;

	return 1-ao;
}

void ComputeDirLight(out vec3 ambient, out vec3 diffuse, out vec3 spec)
{
	if(length( vec3(directionalLight.Intensity.x, directionalLight.Intensity.y, directionalLight.Intensity.z) ) > 0.0)
	{
		vec3 a = vec3(0.0);
		vec3 d = vec3(0.0);
		vec3 s = vec3(0.0);
		phongModelDirLight(a, d, s);
		ambient = a;
		diffuse = d;
		spec    = s;
	}
}
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

	vec3 E = normalize(g_viewPos);

	float diffuseFactor = dot( lightVec, g_normal );

	if(diffuseFactor > 0)
	{
		// For shadows
		vec4 worldPos = inverse(ViewMatrix) * vec4(g_viewPos, 1.0);
		vec4 shadowCoord = BiasMatrix * ShadowViewProj * worldPos;
		float shadow = 1.0;// = textureProj(ShadowDepthTex, shadowCoord);
		// The sum of the comparisons with nearby texels  
		float sum = 0;
		// Sum contributions from texels around ShadowCoord  
		sum += textureProjOffset(ShadowDepthTex, shadowCoord, ivec2(-1,-1));  
		sum += textureProjOffset(ShadowDepthTex, shadowCoord, ivec2(-1,1));  
		sum += textureProjOffset(ShadowDepthTex, shadowCoord, ivec2(1,1));  
		sum += textureProjOffset(ShadowDepthTex, shadowCoord, ivec2(1,-1));  

		float shadowResult = sum * 0.25;
		float bias = min( pow(shadowCoord.z, 10 - 10*shadowCoord.z), 0.40);

		if ( shadowResult < (shadowCoord.z - bias)/shadowCoord.w) 
		{
		   shadow = shadowResult;
		}

		// diffuse
		diffuse = diffuseFactor * thisLightColor * thisLightIntensity.y * shadow;

		// specular
		vec3 v = reflect( lightVec, g_normal );
		float specFactor = pow( max( dot(v, E), 0.0 ), g_shininess );
		spec = specFactor * thisLightColor * thisLightIntensity.z * g_ks * shadow;        
	}

	return;
}

void phongModel(int index, out vec3 ambient, out vec3 diffuse, out vec3 spec)
{
    ambient = vec3(0.0);
    diffuse = vec3(0.0);
    spec    = vec3(0.0);

	vec3 thisLightPosition	= vec3(pointlights[index].Position.x, pointlights[index].Position.y, pointlights[index].Position.z);
	vec3 thisLightColor		= vec3(pointlights[index].Color.x, pointlights[index].Color.y, pointlights[index].Color.z);
	vec3 thisLightIntensity = vec3(pointlights[index].Intensity.x, pointlights[index].Intensity.y, pointlights[index].Intensity.z);

    vec3 lightVec = (ViewMatrix * vec4(thisLightPosition, 1.0)).xyz - g_viewPos;

    float d = length(lightVec);

    if(d > pointlights[index].Range)
        return;
    lightVec /= d; //normalizing
        

	ambient = thisLightColor * thisLightIntensity.x;

	vec3 E = normalize(g_viewPos);

	float diffuseFactor = dot( lightVec, g_normal );

	if(diffuseFactor > 0)
	{
		// diffuse
		diffuse = diffuseFactor * thisLightColor * thisLightIntensity.y;

		// specular
		vec3 v = reflect( lightVec, g_normal );
		float specFactor = pow( max( dot(v, E), 0.0 ), g_shininess );
		spec = specFactor * thisLightColor * thisLightIntensity.z * g_ks;        
	}

	float att = 1 - pow((d/pointlights[index].Range), 1.0f);

	ambient *= att;
	diffuse *= att;
	spec    *= att;

	return;
}

vec3 reconstructPosition(float p_depth)
{ 
	vec2 ndc = vec2(g_threadID) / vec2( gl_WorkGroupSize.xy*gl_NumWorkGroups.xy ) * 2.0f - 1.0f;

    vec4 sPos = vec4(ndc, p_depth*2.0f - 1.0f, 1.0);
    sPos = InvProjection * sPos;

    return (sPos.xyz / sPos.w);
}
