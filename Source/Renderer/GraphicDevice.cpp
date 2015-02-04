#include "GraphicDevice.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "TextureLoader.h"

using namespace Renderer;
using namespace glm;

GraphicDevice::GraphicDevice()
{
	m_SDLinitialized = false;
}

GraphicDevice::GraphicDevice(Camera _camera)
{
	m_camera = new Camera(_camera);
	m_SDLinitialized = true;
}

GraphicDevice::~GraphicDevice()
{
	delete(m_skybox);

	SDL_GL_DeleteContext(m_glContext);
	// Close and destroy the window
	SDL_DestroyWindow(m_window);
	// Clean up
	//SDL_Quit();
}

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
	return TextRenderer::AddFont(filepath, size);
}

float GraphicDevice::CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size)
{
	SDL_Surface* surface = TextRenderer::CreateTextSurface(textString, fontIndex, color);
	if (size.x > 0)
		surface->w = size.x;
	if (size.y > 0)
		surface->h = size.y;
	m_surfaces.push_back(std::pair<std::string, SDL_Surface*>(textureName, surface));

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
	return (float)surface->w / (float)surface->h;
}

void GraphicDevice::CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size)
{
	SDL_Surface* surface = TextRenderer::CreateWrappedTextSurface(textString, fontIndex, color, wrapLength);
	if (size.x > 0)
		surface->w = size.x;
	if (size.y > 0)
		surface->h = size.y;
	m_surfaces.push_back(std::pair<std::string, SDL_Surface*>(textureName, surface));
}

void GraphicDevice::BufferSurfaces()
{
	for (std::pair<std::string, SDL_Surface*> surface : m_surfaces)
	{
		if (m_textures.find(surface.first) != m_textures.end())
			glDeleteTextures(1, &m_textures[surface.first]);
		GLuint texture = TextureLoader::LoadTexture(surface.second, GL_TEXTURE1);
		m_textures[surface.first] = texture;
		m_vramUsage += (surface.second->w * surface.second->h * 4 * 4);
		SDL_FreeSurface(surface.second);
	}
	m_surfaces.clear();
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

void GraphicDevice::BufferModelTextures()
{
	for (ModelTexture modelTexture : m_modelTextures)
	{
		BufferModelTexture(modelTexture.id, modelTexture.textureName, modelTexture.textureType);
	}
	m_modelTextures.clear();
}
