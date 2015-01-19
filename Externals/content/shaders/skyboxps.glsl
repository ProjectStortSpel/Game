#version 430 core

// out values
layout( location = 0 ) out vec4 ColorData;

// in values
in vec2 v_textureCoord;

// uniform textures
uniform sampler2D diffuseTex;

// fragment code
void main(void)
{
	ColorData = texture2D( diffuseTex, v_textureCoord );
}
