#version 100
precision mediump float;

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec3 VertexTangent;
attribute vec3 VertexBiTangent;
attribute vec2 VertexTexCoord;

varying vec2 TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

void main()
{
	TexCoord = VertexTexCoord;
	mat4 MVP = ProjectionMatrix * ModelViewMatrix;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}