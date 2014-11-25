#version 430
#extension GL_ARB_explicit_attrib_location : require

layout( location = 0 ) in vec2 VertexPosition;
out vec2 TexCoord;

void main()
{
	TexCoord = VertexPosition;
	gl_Position = vec4(VertexPosition.x, VertexPosition.y, 0.0f, 1.0f);
}