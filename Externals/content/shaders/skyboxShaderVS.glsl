#version 400

layout (location = 0) in vec3 VertexPoint;
out vec3 TexCoord;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(VertexPoint, 1.0);
	TexCoord = VertexPoint;
}