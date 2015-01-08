#version 430

in vec3 TexCoord;
layout( location = 0 ) out vec4 ColorData;

uniform samplerCube cubemap;

void main()
{
	ColorData = texture(cubemap, TexCoord);//vec4(1.0,1.0,1.0,1.0);//
}