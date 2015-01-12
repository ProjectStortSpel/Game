#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 v_textureCoord;

float2 gWindowSize;

vec2 gPosition;
vec2 gSize;
vec2 gOrigo;
vec4 gTexCoord;
vec4 gColor;
float gRotation;

void main() 
{
	vec2 vTexCoords[4] = {	vec2(gTexCoord.x, gTexCoord.y),
							vec2(gTexCoord.z, gTexCoord.y),
							vec2(gTexCoord.x, gTexCoord.w),
							vec2(gTexCoord.z, gTexCoord.w) };

	vec4 vPositions[4] = {	vec4(0,			0,			0, 1),
							vec4(gSize.x,	0,			0, 1),
							vec4(0,			gSize.y,	0, 1),
							vec4(gSize.x,	gSize.y,	0, 1) };
	
	mat4 Origo = {	1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 0, 0,
					-gOrigo.x, -gOrigo.y, 0, 1 };

	mat4 Rotate = {	cos(gRotation), -sin(gRotation), 0, 0,
					sin(gRotation),  cos(gRotation), 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 1 };

	mat4 Translate = {	1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 0, 0,
						gPosition.x, gPosition.y, 0, 1 };

	mat4 ClipSpace = {	2/gWindowSize.x, 0, 0, 0,
						0, -2/gWindowSize.y, 0, 0,
						0, 0, 0, 0,
						-1, 1, 0, 1 };


	for (int i = 0; i < 4; i++)
	{
		gl_Position = vPositions[i] * Origo * Rotate * Translate * ClipSpace;
		v_textureCoord = vTexCoords[i];
		EmitVertex();
	}

    EndPrimitive(); 
}