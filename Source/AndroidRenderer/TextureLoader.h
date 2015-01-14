/*Author: Chris*/
#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_
#include "stdafx.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbimage/stb_image.h>

class TextureLoader{
public:
static unsigned int LoadTexture(const char* file, GLenum textureSlot, int &height, int &width)
{
	std::string fp = std::string(file);
	fp.erase(std::remove(fp.begin(), fp.end(), 13), fp.end());
	// Open file
	SDL_RWops* fileIn = SDL_RWFromFile(fp.c_str(), "rb");
	if (fileIn == NULL)
		SDL_Log("File %s not found", fp.c_str());
	// Get file length
	Sint64 length = SDL_RWseek(fileIn, 0, RW_SEEK_END);
	if (length <= 0)
		SDL_Log("Length of file %s lower than or equal to zero", fp.c_str());
	SDL_RWseek(fileIn, 0, RW_SEEK_SET);
	// Read data
	char* data = new char[length];
	SDL_RWread(fileIn, data, length, 1);
	// Close file
	SDL_RWclose(fileIn);
  
	int channels;
		// Load texture file and convert to openGL format
	//unsigned char* imgData = stbi_load(file, &width, &height, &channels, STBI_rgb_alpha);
	unsigned char* imgData = stbi_load_from_memory((const unsigned char*)data, (int)length, &width, &height, &channels, STBI_rgb_alpha);

	if (!imgData)
	{
		SDL_Log( "Texture '%s' not loaded." , fp.c_str());
	}

	GLuint texHandle;
	// Copy file to OpenGL
	glActiveTexture(textureSlot);
	glGenTextures(1, &texHandle);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//gluBuild2DMipmaps(GL_TEXTURE_2D, channels, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

	stbi_image_free(imgData);
	delete data;
	return texHandle;
}

static unsigned int LoadCubeMap(const char* file, GLenum textureSlot, int &height, int &width)
{
	GLuint texHandle;
	glActiveTexture(textureSlot);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &texHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texHandle);

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	std::string suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; i++) 
	{
		std::stringstream texName; 
		texName << file << "_" << suffixes[i] << ".png";

		int channels;
		// Load texture file and convert to openGL format
		unsigned char* imgData = stbi_load(texName.str().c_str(), &width, &height, &channels, STBI_rgb_alpha);

		if (!imgData)
			SDL_Log("Cubemap texture '%s' not loaded.", texName.str().c_str() );

		glTexImage2D(targets[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

		delete imgData;
	}

	return texHandle;
}

};

#endif