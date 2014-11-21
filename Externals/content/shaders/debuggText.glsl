#version 430 core
#extension GL_EXT_texture_array : enable
#extension GL_NV_texture_array : enable

struct PointLight
{ 
	vec4 Diffuse;
};

// Texture
uniform sampler2D v_Texture;

// output 
layout (rgba32f, binding = 0) uniform image2D output_image;

void main(void)
{
	// Sample from imagebuffers
    //vec3 ray_d			= imageLoad(ray_dirbuffer, ivec2(gl_GlobalInvocationID.xy)).xyz;
    //vec3 ray_o			= imageLoad(ray_posbuffer, ivec2(gl_GlobalInvocationID.xy)).xyz;
    //vec4 d				= imageLoad(hitbuffer, ivec2(gl_GlobalInvocationID.xy)).xyzw;
	//vec3 prev_color		= imageLoad(output_image, ivec2(gl_GlobalInvocationID.xy)).xyz;

	imageStore(
		output_image,
		ivec2(gl_GlobalInvocationID.xy),
		vec4(0.0, 0.0, 1.0, 0.0)
	);
}