#version 400
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

out vec3 Normal;
out vec3 Tan;
out vec3 BiTan;
out vec2 TexCoord;
out vec3 ViewPos;

const int nrOfJoints = 36;
uniform mat4 anim[nrOfJoints];

void main()
{
	vec4 weights = normalize(VertexJointWeight);

	mat4 skin = mat4(0);
	skin += anim[int(VertexJointIndex.x)] * weights.x;
	skin += anim[int(VertexJointIndex.y)] * weights.y;
	skin += anim[int(VertexJointIndex.z)] * weights.z;
	skin += anim[int(VertexJointIndex.w)] * weights.w;
    
	//skin += mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, weights.x, 0, 1);

	ViewPos = (V * M * skin * vec4(VertexPosition, 1.0)).xyz;
	gl_Position = P * V * M * skin * vec4(VertexPosition, 1.0);

	Normal = normalize( NormalMatrix * (skin * vec4(VertexNormal, 0.0)).xyz );
	Tan = normalize( NormalMatrix * (skin * vec4(VertexTangent, 0.0)).xyz );
	BiTan = normalize( NormalMatrix * (skin * vec4(VertexBiTangent, 0.0)).xyz );
	TexCoord = VertexTexCoord;
}