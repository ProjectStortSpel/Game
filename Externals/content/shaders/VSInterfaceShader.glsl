#version 400
layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec3 VertexNormal;
layout( location = 2 ) in vec3 VertexTangent;
layout( location = 3 ) in vec3 VertexBiTangent;
layout( location = 4 ) in vec2 VertexTexCoord;
layout( location = 5 ) in mat4 ModelViewMatrix;
// used locations 5-8
layout( location = 9 ) in mat3 NormalMatrix;
// used locations 9-11
layout( location = 12 ) in vec3 ColorVec;

out vec2 TexCoord;
out vec3 AddColor;

uniform mat4 ProjectionMatrix;

void main()
{
	TexCoord = VertexTexCoord;
	mat4 MVP = ProjectionMatrix * ModelViewMatrix;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
	AddColor = ColorVec;
}