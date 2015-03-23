#version 100
#extension OES_fragment_precision_high : enable

precision highp float;

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec3 VertexTangent;
attribute vec3 VertexBiTangent;
attribute vec2 VertexTexCoord;


uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(VertexPosition,1.0);
}