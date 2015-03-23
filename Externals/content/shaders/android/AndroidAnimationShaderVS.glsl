#version 100

precision highp float;

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec3 VertexTangent;
attribute vec3 VertexBiTangent;
attribute vec2 VertexTexCoord;
attribute vec4 VertexJointIndex;
attribute vec4 VertexJointWeight;

varying vec3 Normal;
varying vec3 Tan;
varying vec3 BiTan;
varying vec2 TexCoord;
varying vec4 ViewPos;

uniform mat4 MV;
uniform mat4 P;
uniform mat3 NormalMatrix;

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
    
	//skin += mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, weights.x, 0, 1);

	ViewPos = MV * vec4(VertexPosition, 1.0); //MV * skin * vec4(VertexPosition, 1.0);
	gl_Position = P * MV * skin * vec4(VertexPosition, 1.0);

	Normal = normalize( NormalMatrix * (skin * vec4(VertexNormal, 0.0)).xyz );
	Tan = normalize( NormalMatrix * (skin * vec4(VertexTangent, 0.0)).xyz );
	BiTan = normalize( NormalMatrix * (skin * vec4(VertexBiTangent, 0.0)).xyz );
	TexCoord = VertexTexCoord;
}