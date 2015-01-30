#include "GraphicDevice.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "TextureLoader.h"

using namespace Renderer;
using namespace glm;


void GraphicDevice::PollEvent(SDL_Event _event)
{
	switch (_event.type)
	{
	case SDL_WINDOWEVENT:
		switch (_event.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			int w, h;
			SDL_GetWindowSize(m_window, &w, &h);
			ResizeWindow(w,h);
			break;
		}
		break;

	default:
		break;
	}
}

void GraphicDevice::ResizeWindow(int _width, int _height)
{
	// GRAPHIC CARD WORK GROUPS OF 16x16
	int x, y;
	x = _width / 16;
	if (x == 0) x = 1;
	y = _height / 16;
	if (y == 0) y = 1;
	m_clientWidth = x * 16;
	m_clientHeight = y * 16;

	std::cout << m_clientWidth << "x" << m_clientHeight << std::endl;

	SDL_SetWindowSize(m_window, m_clientWidth, m_clientHeight);
}

bool GraphicDevice::InitSkybox()
{
	int w, h;
	GLuint texHandle = TextureLoader::LoadCubeMap("content/textures/skybox", GL_TEXTURE1, w, h);
	if (texHandle < 0)
		return false;

	m_skyBoxShader.UseProgram();
	m_skybox = new SkyBox(texHandle, m_camera->GetFarPlane());
	m_vramUsage += (w*h * 6 * 4 * sizeof(float));

	return true;
}

int GraphicDevice::AddFont(const std::string& filepath, int size)
{
	return m_sdlTextRenderer.AddFont(filepath, size);
}

float GraphicDevice::CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size)
{
	if (m_textures.find(textureName) != m_textures.end())
		glDeleteTextures(1, &m_textures[textureName]);
	SDL_Surface* surface = m_sdlTextRenderer.CreateTextSurface(textString, fontIndex, color);
	if (size.x > 0)
		surface->w = size.x;
	if (size.y > 0)
		surface->h = size.y;

	//int numPix = surface->h * surface->w;

	//Uint8* pixels = (Uint8*)surface->pixels;
	//int index = 0;
	//for (int i = 0; i < numPix; ++i)
	//{
	//	index = i * 4;
	//	
	//	if (pixels[index + 3] == 0)
	//	{
	//		pixels[index + 0] = 0;
	//		pixels[index + 1] = 0;
	//		pixels[index + 2] = 0;
	//	}
	//}

	//std::stringstream ss;
	//ss << "content/";
	//ss << textureName;
	//ss << ".bmp";


	//SDL_SaveBMP(surface, ss.str().c_str());

	//m_deferredShader1.UseProgram();
	GLuint texture = TextureLoader::LoadTexture(surface, GL_TEXTURE1);
	m_textures[textureName] = texture;
	m_vramUsage += (surface->w * surface->h * 4 * 4);
	SDL_FreeSurface(surface);
	return (float)surface->w / (float)surface->h;
}

void GraphicDevice::CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size)
{
	if (m_textures.find(textureName) != m_textures.end())
		glDeleteTextures(1, &m_textures[textureName]);
	SDL_Surface* surface = m_sdlTextRenderer.CreateWrappedTextSurface(textString, fontIndex, color, wrapLength);
	if (size.x > 0)
		surface->w = size.x;
	if (size.y > 0)
		surface->h = size.y;
	//m_deferredShader1.UseProgram();
	GLuint texture = TextureLoader::LoadTexture(surface, GL_TEXTURE1);
	m_textures[textureName] = texture;
	m_vramUsage += (surface->w * surface->h * 4 * 4);
	SDL_FreeSurface(surface);
}

GLuint GraphicDevice::AddTexture(std::string _fileDir, GLenum _textureSlot)
{
	for (std::map<const std::string, GLuint>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		if (it->first == _fileDir)
			return it->second;
	}
	int texSizeX, texSizeY;
	GLuint texture = TextureLoader::LoadTexture(_fileDir.c_str(), _textureSlot, texSizeX, texSizeY);
	m_textures.insert(std::pair<const std::string, GLenum>(_fileDir, texture));
	m_vramUsage += (texSizeX * texSizeY * 4 * 4);
	return texture;
}