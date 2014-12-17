#version 430
layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec3 VertexNormal;
layout( location = 2 ) in vec3 VertexTangent;
layout( location = 3 ) in vec3 VertexBiTangent;
layout( location = 4 ) in vec2 VertexTexCoord;
layout( location = 5 ) in mat4 MVP;
// used locations 5-8
layout( location = 9 ) in mat3 NormalMatrix;
// used locations 9-12

out vec2 TexCoord;


//Matrices-----------------------------------------
uniform mat4 ProjectionMatrix;	

void main()
{
	TexCoord = VertexTexCoord;
	gl_Position = MVP * vec4(VertexPosition,1.0);
}