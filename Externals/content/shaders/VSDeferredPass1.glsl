#version 430
layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec3 VertexNormal;
layout( location = 2 ) in vec2 VertexTexCoord;
//layout( location = 3 ) in vec3 VertexTangent;
//layout( location = 4 ) in vec2 VertexBiTangent;

out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	Normal = normalize( NormalMatrix * VertexNormal);
	TexCoord = VertexTexCoord;
	mat4 MVP = ProjectionMatrix * ModelViewMatrix;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}