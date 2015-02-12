#include "GraphicDevice.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "TextureLoader.h"

using namespace Renderer;
using namespace glm;

GraphicDevice::GraphicDevice()
{
	m_windowPosX = 70;
	m_windowPosY = 2;
	m_windowCaption = "Project MOMS SPAGHETTI";
	m_SDLinitialized = false;
	
	m_pointerToPointlights = NULL;
	m_pointerToDirectionalLights = NULL;
	m_numberOfPointlights = 0;
	m_numberOfDirectionalLights = 0;
}

GraphicDevice::GraphicDevice(Camera _camera, int x, int y)
{
	m_camera = new Camera(_camera);
	m_windowPosX = x;
	m_windowPosY = y;
	m_windowCaption = "Project MOMS SPAGHETTI";
	m_SDLinitialized = true;

	m_pointerToPointlights = NULL;
	m_pointerToDirectionalLights = NULL;
	m_numberOfPointlights = 0;
	m_numberOfDirectionalLights = 0;
}

GraphicDevice::~GraphicDevice()
{
	delete(m_skybox);
	delete m_pointerToPointlights;

	for (int i = 0; i < m_particleSystems.size(); i++)
	{
		delete m_particleSystems[i];
	}
	m_particleSystems.clear();

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

void GraphicDevice::GetWindowPos(int &x, int &y)
{
	int posx, posy;
	SDL_GetWindowPosition(m_window, &posx, &posy);
	x = posx;
	y = posy;
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
		int oldTexture = -1;
		if (m_textures.find(surface.first) != m_textures.end())
		{
			oldTexture = m_textures[surface.first];
			glDeleteTextures(1, &m_textures[surface.first]);
		}
		GLuint texture = TextureLoader::LoadTexture(surface.second, GL_TEXTURE1);
		m_textures[surface.first] = texture;
		m_vramUsage += (surface.second->w * surface.second->h * 4 * 4);
		SDL_FreeSurface(surface.second);
		if (oldTexture != -1)
			UpdateTextureIndex(texture, oldTexture);
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

struct sort_depth
{
	inline bool operator() (const Model& a, const Model& b)
	{
		return (*a.instances[0].modelMatrix)[3][2] < (*b.instances[0].modelMatrix)[3][2];
	}
};

void GraphicDevice::SortModelsBasedOnDepth(std::vector<Model>* models)
{
	std::sort(models->begin(), models->end(), sort_depth());
}
