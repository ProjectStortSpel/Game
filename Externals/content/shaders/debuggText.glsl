#version 430 core
#extension GL_EXT_texture_array : enable
#extension GL_NV_texture_array : enable

// UNIFORMS
layout (local_size_x = 8, local_size_y = 16) in;

// output 
layout (rgba32f, binding = 0) uniform image2D output_image;

layout (std430, binding = 1) buffer debugtext { int letter []; };

void main(void)
{
	uint x = gl_WorkGroupID.x;
	uint y = (gl_NumWorkGroups.y-1)-gl_WorkGroupID.y;

	uint xy = x + y * gl_NumWorkGroups.x;

	vec4 something = vec4(0,float(letter[xy].x)/4608.f * 0.5,0,0);

	imageStore(
		output_image,
		ivec2(gl_GlobalInvocationID.xy),
		something
	);
}