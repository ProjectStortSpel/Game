#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_
#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbimage/stb_image.h>

class TextureLoader{
public:
static unsigned int LoadTexture(char* file, GLenum textureSlot)
{
	int width, height, channels;
		// Load texture file and convert to openGL format
	unsigned char* imgData = stbi_load(file, &width, &height, &channels, STBI_rgb_alpha);

	if (!imgData)
	{
		printf("ERROR: Texture not loaded \n");
	}

	GLuint texHandle;
	// Copy file to OpenGL
	glActiveTexture(textureSlot);
	glGenTextures(1, &texHandle);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//gluBuild2DMipmaps(GL_TEXTURE_2D, channels, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

	stbi_image_free(imgData);
	return texHandle;
}

};

#endif