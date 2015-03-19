#version 100

precision highp float;

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec3 VertexTangent;
attribute vec3 VertexBiTangent;
attribute vec2 VertexTexCoord;

varying vec3 Normal;
varying vec3 Tan;
varying vec3 BiTan;
varying vec2 TexCoord;
varying vec4 ViewPos;

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform float ElapsedTime;

void main()
{
	Normal = normalize(NormalMatrix * VertexNormal);
	Tan = normalize( NormalMatrix * VertexTangent);
	BiTan = normalize( NormalMatrix * VertexBiTangent);
	
	float texCoordY = VertexTexCoord.y + 0.4*ElapsedTime;
	TexCoord = vec2(VertexTexCoord.x, texCoordY);

	ViewPos = ModelViewMatrix * vec4(VertexPosition, 1.0);
	mat4 MVP = ProjectionMatrix * ModelViewMatrix;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}
