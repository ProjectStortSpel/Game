#version 440
layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec3 VertexNormal;
layout( location = 2 ) in vec3 VertexTangent;
layout( location = 3 ) in vec3 VertexBiTangent;
layout( location = 4 ) in vec2 VertexTexCoord;
layout( location = 5 ) in vec4 VertexJointIndex;
layout( location = 6 ) in vec4 VertexJointWeight;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat3 NormalMatrix;

struct JointMatrix
{
	float x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3;
};

layout (std430, binding = 6) buffer Animation   
{
	JointMatrix anim[];
};

out vec3 Normal;
out vec3 Tan;
out vec3 BiTan;
out vec2 TexCoord;

mat4 JointToMatrix(int i)
{
	return mat4(anim[i].x0, anim[i].y0, anim[i].z0, anim[i].w0, anim[i].x1, anim[i].y1, anim[i].z1, anim[i].w1, anim[i].x2, anim[i].y2, anim[i].z2, anim[i].w2, anim[i].x3, anim[i].y3, anim[i].z3, anim[i].w3);
}

void main()
{
	vec4 weights = normalize(VertexJointWeight);

	mat4 skin = mat4(0);
	skin += JointToMatrix(int(VertexJointIndex.x)) * weights.x;
	skin += JointToMatrix(int(VertexJointIndex.y)) * weights.y;
	skin += JointToMatrix(int(VertexJointIndex.z)) * weights.z;
	skin += JointToMatrix(int(VertexJointIndex.w)) * weights.w;
    
	//skin += mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, weights.y, 0, 1);
	
	gl_Position = P * V * M * skin * vec4(VertexPosition, 1.0);

	Normal = normalize( NormalMatrix * (skin * vec4(VertexNormal, 0.0)).xyz );
	Tan = normalize( NormalMatrix * (skin * vec4(VertexTangent, 0.0)).xyz );
	BiTan = normalize( NormalMatrix * (skin * vec4(VertexBiTangent, 0.0)).xyz );
	TexCoord = VertexTexCoord;
}