#version 140

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
//attribute vec3 VertexTangent;
//attribute vec3 VertexBiTangent;
//attribute vec2 VertexTexCoord;

//varying vec3 Normal;
//varying vec3 Tan;
//varying vec3 BiTan;
//varying vec2 TexCoord;
//varying vec3 ViewPos;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
//uniform mat3 NormalMatrix;

void main()
{
	Normal = normalize( NormalMatrix * VertexNormal);
	//Tan = normalize( NormalMatrix * VertexTangent);
	//BiTan = normalize( NormalMatrix * VertexBiTangent);
	//TexCoord = VertexTexCoord;
	//ViewPos = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;
	mat4 MVP = ProjectionMatrix * ModelViewMatrix;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}
