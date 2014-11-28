#version 430 core

// UNIFORMS
layout (local_size_x = 16, local_size_y = 16) in;

//read texture 
layout (rgba8, binding = 0) uniform image2D input_image;

// output 
layout (rgba32f, binding = 1) uniform image2D output_image;


void main(void)
{
	vec4 outData = imageLoad(input_image, ivec2(gl_GlobalInvocationID.xy));
	//outData = vec4(1.0, 0.2, 0.2, 1.0);

	//imageStore(
	//	output_image,
	//	ivec2(gl_GlobalInvocationID.xy),
	//	something
	//);
	imageStore(
		output_image,
		ivec2(gl_GlobalInvocationID.xy),
		outData
	);
}