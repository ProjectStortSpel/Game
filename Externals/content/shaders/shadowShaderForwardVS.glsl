#version 400
layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec3 VertexNormal;
layout( location = 2 ) in vec3 VertexTangent;
layout( location = 3 ) in vec3 VertexBiTangent;
layout( location = 4 ) in vec2 VertexTexCoord;
layout( location = 7 ) in mat4 MVP;
// used locations 7-10
layout( location = 11 ) in mat3 NormalMatrix;
// used locations 11-13

out vec2 TexCoord;


//Matrices-----------------------------------------
uniform mat4 ProjectionMatrix;	

void main()
{
	TexCoord = VertexTexCoord;
	gl_Position = MVP * vec4(VertexPosition,1.0);
}