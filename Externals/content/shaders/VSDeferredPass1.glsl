#version 440
layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec3 VertexNormal;
layout( location = 2 ) in vec3 VertexTangent;
layout( location = 3 ) in vec3 VertexBiTangent;
layout( location = 4 ) in vec2 VertexTexCoord;
layout( location = 7 ) in mat4 ModelViewMatrix;
// used locations 7-10
layout( location = 11 ) in mat3 NormalMatrix;
// used locations 11-13
layout( location = 14 ) in vec3 ColorVec;

//flat out int instanceID;

out vec3 Normal;
out vec3 Tan;
out vec3 BiTan;
out vec2 TexCoord;
out vec3 AddColor;

uniform mat4 ProjectionMatrix;

void main()
{
	Normal = normalize( NormalMatrix * VertexNormal);
	Tan = normalize( NormalMatrix * VertexTangent);
	BiTan = normalize( NormalMatrix * VertexBiTangent);
	TexCoord = VertexTexCoord;
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(VertexPosition, 1.0);
	AddColor = ColorVec;
	//instanceID = gl_InstanceID;
}