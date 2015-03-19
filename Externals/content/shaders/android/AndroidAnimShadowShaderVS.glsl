#version 100

precision highp float;

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec3 VertexTangent;
attribute vec3 VertexBiTangent;
attribute vec2 VertexTexCoord;
attribute vec4 VertexJointIndex;
attribute vec4 VertexJointWeight;

varying vec2 TexCoord;

uniform mat4 MVP;

const int nrOfJoints = 36;
uniform mat4 anim[nrOfJoints];

void main()
{
	vec4 weights = normalize(VertexJointWeight);

	mat4 skin = mat4(0.0);
	skin += anim[int(VertexJointIndex.x)] * weights.x;
	skin += anim[int(VertexJointIndex.y)] * weights.y;
	skin += anim[int(VertexJointIndex.z)] * weights.z;
	skin += anim[int(VertexJointIndex.w)] * weights.w;
    
	gl_Position = MVP * skin * vec4(VertexPosition, 1.0);
	TexCoord = VertexTexCoord;
}