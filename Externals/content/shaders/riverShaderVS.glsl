#version 400
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

out vec3 Normal;
out vec3 Tan;
out vec3 BiTan;
out vec2 TexCoord;
out vec3 ViewPos;
out vec3 AddColor;

uniform mat4 ProjectionMatrix;
uniform float ElapsedTime;

void main()
{
	Normal = normalize( NormalMatrix * VertexNormal);
	Tan = normalize( NormalMatrix * VertexTangent);
	BiTan = normalize( NormalMatrix * VertexBiTangent);

	float texCoordY = VertexTexCoord.y + 0.4f*ElapsedTime;
	TexCoord = vec2(VertexTexCoord.x, texCoordY);

	ViewPos = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;
	mat4 MVP = ProjectionMatrix * ModelViewMatrix;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
	AddColor = ColorVec;
}