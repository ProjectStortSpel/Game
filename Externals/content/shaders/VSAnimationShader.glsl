#version 440
layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec3 VertexNormal;
layout( location = 2 ) in vec3 VertexTangent;
layout( location = 3 ) in vec3 VertexBiTangent;
layout( location = 4 ) in vec2 VertexTexCoord;
layout( location = 5 ) in vec4 VertexJointIndex;
layout( location = 6 ) in vec4 VertexJointWeight;
// used locations 5-8
// used locations 9-12

uniform mat4 M;
uniform mat4 VP;
uniform mat3 NormalMatrix;

struct JointMatrix
{
	float x0, y0, z0, w0;
	float x1, y1, z1, w1;
	float x2, y2, z2, w2;
	float x3, y3, z3, parent;
};

//flat out int instanceID;
layout (std430, binding = 1) buffer Joints   
{
	JointMatrix joints[];
};


out vec3 Normal;
out vec3 Tan;
out vec3 BiTan;
out vec2 TexCoord;

mat4 JointToMatrix(JointMatrix joint)
{
	return mat4(
				joint.x0, joint.y0, joint.z0, joint.w0,
				joint.x1, joint.y1, joint.z1, joint.w1,
				joint.x2, joint.y2, joint.z2, joint.w2,
				joint.x3, joint.y3, joint.z3, 1
				);
}

void main()
{
	Normal = normalize( NormalMatrix * VertexNormal );
	Tan = normalize( NormalMatrix * VertexTangent);
	BiTan = normalize( NormalMatrix * VertexBiTangent);
	TexCoord = VertexTexCoord;
	uint jointIndex = uint(VertexJointIndex.x);
	gl_Position = VP * M * JointToMatrix(joints[0]) * vec4(VertexPosition, 1.0);
	//instanceID = gl_InstanceID;
}