#version 430 core
#extension GL_EXT_texture_array : enable
#extension GL_NV_texture_array : enable

// UNIFORMS
layout (local_size_x = 16, local_size_y = 16) in;

// output 
layout (rgba32f, binding = 0) uniform image2D output_image;


struct charecter
{ 
	int id;
};
layout (std430, binding = 1) buffer DebuggString { charecter debuggtext []; };

void main(void)
{
	
	vec4 something = vec4(float(debuggtext[gl_GlobalInvocationID.x].id)*0.01);

	imageStore(
		output_image,
		ivec2(gl_GlobalInvocationID.xy),
		something
	);
}