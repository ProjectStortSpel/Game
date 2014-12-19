#version 430
in vec3 Normal;
in vec3 Tan;
in vec3 BiTan;
in vec2 TexCoord;

//flat in int instanceID;

// The g-buffer textures
layout(binding = 0) uniform sampler2D NormalTex;
layout(binding = 1) uniform sampler2D ColorTex;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform int TexFlag;

layout (location = 0) out vec4 NormalData;
layout (location = 1) out vec4 ColorData;

void main() 
{
	// -- INPUTS --
	// Sample maps
	vec3 color_map = texture( diffuseTex, TexCoord ).rgb;
	vec3 normal_map = texture( normalTex, TexCoord ).rgb;
	vec3 specglow_map = texture( specularTex, TexCoord ).rgb;

	
	if(TexFlag == 0) // everything
	{
		// -- OUTPUTS --
		// Set Color output
		ColorData.xyz = color_map.xyz;								// rgb = color
		ColorData.w = specglow_map.z;								// a = glow
		// Set Normal output
		normal_map = (normal_map * 2.0f) - 1.0f;
		mat3 texSpace = mat3(Tan, BiTan, Normal);
		vec3 allNormalData = normalize( texSpace * normal_map );
		NormalData.xy = allNormalData.xy;	//+allNormalData.xy-allNormalData.xy;							// rg = normal
		NormalData.z = specglow_map.x;								// b = spec reflec
		if(allNormalData.z > 0)
			NormalData.w = specglow_map.y*0.5;						// a = spec shine + normal.z flag
		else
			NormalData.w = specglow_map.y*0.5 + 0.5;
			
	}
	else if(TexFlag == 1) //diffuse only
	{

		ColorData.xyz = color_map.xyz;					// rgb = color
		ColorData.w = 0.0;								// a = glow

		NormalData.xy = Normal.xy;						// rg = normal
		NormalData.z = 0.0;								// b = spec reflec
		float shine = 0.0;
		if(Normal.z > 0)
			NormalData.w = shine*0.5;						// a = spec shine + normal.z flag
		else
			NormalData.w = shine*0.5 + 0.5;
	}
	else if(TexFlag == 2) //normal only
	{
		ColorData.xyz = vec3(0.5, 0.5, 0.8);					// rgb = color
		ColorData.w = 0.0;										// a = glow

		normal_map = (normal_map * 2.0f) - 1.0f;
		mat3 texSpace = mat3(Tan, BiTan, Normal);
		vec3 allNormalData = normalize( texSpace * normal_map );
		NormalData.xy = allNormalData.xy;
		NormalData.z = 0.4;										// b = spec reflec
		float shine = 0.05;										// a = spec shine
		if(allNormalData.z > 0)
			NormalData.w = shine*0.5;						// a = spec shine + normal.z flag
		else
			NormalData.w = shine*0.5 + 0.5;
	}
	else if(TexFlag == 3) //specular only
	{
		ColorData.xyz = vec3(0.5, 0.5, 0.8);			// rgb = color
		ColorData.w = 0.0;								// a = glow

		NormalData.xy = Normal.xy;						// rg = normal
		NormalData.z = specglow_map.x;					// b = spec reflec
		if(Normal.z > 0)
			NormalData.w = specglow_map.y*0.5;						// a = spec shine + normal.z flag
		else
			NormalData.w = specglow_map.y*0.5 + 0.5;
	}
	else if(TexFlag == 4) //glow only
	{
		ColorData.xyz = vec3(0.5, 0.5, 0.8);			// rgb = color
		ColorData.w = specglow_map.z;					// a = glow
		NormalData.xy = Normal.xy;						// rg = normal
		NormalData.z = 0.0;								// b = spec reflec
		float shine = 0.0;										// a = spec shine
		if(Normal.z > 0)
			NormalData.w = shine*0.5;						// a = spec shine + normal.z flag
		else
			NormalData.w = shine*0.5 + 0.5;
	}
}