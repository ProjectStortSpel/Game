/* ANDROID VERSION */
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
	m_pointlightsPtr = 0;
}

GraphicDevice::GraphicDevice(Camera _camera)
{
	m_camera = new Camera(_camera);
	m_SDLinitialized = true;
	m_pointlightsPtr = 0;
}

GraphicDevice::~GraphicDevice()
{
	delete(m_camera);
	delete(m_skybox);

	if (m_pointlightsPtr)
		delete m_pointlightsPtr;

	SDL_GL_DeleteContext(m_glContext);
	// Close and destroy the window
	SDL_DestroyWindow(m_window);
	// Clean up
	//SDL_Quit();
	SDL_Log("Graphics D E S T R U C T O R");
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
	/*int x, y;
	x = _width / 16;
	if (x == 0) x = 1;
	y = _height / 16;
	if (y == 0) y = 1;
	m_clientWidth = x * 16;
	m_clientHeight = y * 16;

	std::cout << m_clientWidth << "x" << m_clientHeight << std::endl;*/

	m_clientWidth = _width;
	m_clientHeight = _height;
	SDL_Log("W: %d H: %d", m_clientWidth, m_clientHeight);

	SDL_SetWindowSize(m_window, m_clientWidth, m_clientHeight);
}

bool GraphicDevice::InitSkybox()
{
	int w, h;
	GLuint texHandle = TextureLoader::LoadCubeMap("content/textures/skybox", GL_TEXTURE1, w, h);
	if (texHandle < 0)
		return false;

	m_skyBoxShader.UseProgram();
	GLuint loc = glGetAttribLocation(m_skyBoxShader.GetShaderProgram(), "VertexPoint");
	m_skybox = new SkyBox(texHandle, m_camera->GetFarPlane(), loc);

	return true;
}

GLuint GraphicDevice::AddTexture(std::string _fileDir, GLenum _textureSlot)
{
    //printf("fileDir: %s\n", _fileDir.c_str());
	for (std::map<const std::string, GLuint>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		if (it->first == _fileDir)
			return it->second;
	}
	int texSizeX, texSizeY;
	GLuint texture = TextureLoader::LoadTexture(_fileDir.c_str(), _textureSlot, texSizeX, texSizeY);
	m_textures.insert(std::pair<const std::string, GLenum>(_fileDir, texture));
	return texture;
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
		SDL_FreeSurface(surface.second);
		if (oldTexture != -1)
		{
			for (Model& m : m_modelsForward)
				if (m.texID == oldTexture)
					m.texID = texture;
			for (Model& m : m_modelsViewspace)
				if (m.texID == oldTexture)
					m.texID = texture;
			for (Model& m : m_modelsInterface)
				if (m.texID == oldTexture)
					m.texID = texture;
		}
	}
	m_surfaces.clear();
}

void GraphicDevice::BufferModelTextures()
{
	for (ModelTexture modelTexture : m_modelTextures)
	{
		BufferModelTexture(modelTexture.id, modelTexture.textureName, modelTexture.textureType);
	}
	m_modelTextures.clear();
}

bool GraphicDevice::BufferModelTexture(int _id, std::string _fileDir, int _textureType)
{
  // Temp Model
	Model model;

	bool found = false;
	int renderType;

	// Find model
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].id == _id)
		{
			model = Model(
				m_modelsForward[i].bufferPtr,
				m_modelsForward[i].texID,
				m_modelsForward[i].norID,
				m_modelsForward[i].speID,
				m_modelsForward[i].id,
				m_modelsForward[i].active,
				m_modelsForward[i].modelMatrix,
				m_modelsForward[i].color
				);
			found = true;
			renderType = RENDER_FORWARD;
			m_modelsForward.erase(m_modelsForward.begin() + i);
		}
		if (found) break;
	}
	if (!found)
	{
		for (int i = 0; i < m_modelsViewspace.size(); i++)
		{
			if (m_modelsViewspace[i].id == _id)
			{
				model = Model(
					m_modelsViewspace[i].bufferPtr,
					m_modelsViewspace[i].texID,
					m_modelsViewspace[i].norID,
					m_modelsViewspace[i].speID,
					m_modelsViewspace[i].id,
					m_modelsViewspace[i].active,
					m_modelsViewspace[i].modelMatrix,
					m_modelsViewspace[i].color
					);
				found = true;
				renderType = RENDER_VIEWSPACE;
				m_modelsViewspace.erase(m_modelsViewspace.begin() + i);
			}
			if (found) break;
		}
	}
	if (!found)
	{
		for (int i = 0; i < m_modelsInterface.size(); i++)
		{
			if (m_modelsInterface[i].id == _id)
			{
				model = Model(
					m_modelsInterface[i].bufferPtr,
					m_modelsInterface[i].texID,
					m_modelsInterface[i].norID,
					m_modelsInterface[i].speID,
					m_modelsInterface[i].id,
					m_modelsInterface[i].active,
					m_modelsInterface[i].modelMatrix,
					m_modelsInterface[i].color
					);
				found = true;
				renderType = RENDER_INTERFACE;
				m_modelsInterface.erase(m_modelsInterface.begin() + i);
			}
			if (found) break;
		}
	}
	// Didn't we find it return false
	if (!found) return false;

	// Set new Texture to TextureType
	if (_textureType == TEXTURE_DIFFUSE)
	{
		model.texID = AddTexture(_fileDir, GL_TEXTURE1);
		//m_deferredShader1.CheckUniformLocation("diffuseTex", 1);
	}
	else if (_textureType == TEXTURE_NORMAL)
	{
		model.norID = AddTexture(_fileDir, GL_TEXTURE2);
		//m_deferredShader1.CheckUniformLocation("normalTex", 2);
	}
	else if (_textureType == TEXTURE_SPECULAR)
	{
		model.speID = AddTexture(_fileDir, GL_TEXTURE3);
		//m_deferredShader1.CheckUniformLocation("specularTex", 3);
	}

	// Check if our new Model type already exists and add instance
	if (renderType == RENDER_FORWARD)
	{
		m_modelsForward.push_back(model);
		return true;
	}
	else if (renderType == RENDER_VIEWSPACE)
	{
		m_modelsViewspace.push_back(model);
		return true;
	}
	else if (renderType == RENDER_INTERFACE)
	{
		m_modelsInterface.push_back(model);
		return true;
	}
	else
		SDL_Log("Change texture to incorrect renderType.");

	return false;
}

void GraphicDevice::BufferModel(int _modelId, ModelToLoad* _modelToLoad)
{
	Shader *shaderPtr = NULL;

	if (_modelToLoad->RenderType == RENDER_FORWARD)
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_VIEWSPACE)
	{
		shaderPtr = &m_viewspaceShader;
		m_viewspaceShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_INTERFACE)
	{
		shaderPtr = &m_interfaceShader;
		m_interfaceShader.UseProgram();
	}
	else
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
		//SDL_Log("Deferred requested. Selecting FORWARD");
	}

	// Import Object
	ObjectData obj = ModelLoader::importObject(_modelToLoad->Dir, _modelToLoad->File);

	// Import Texture
	GLuint texture = AddTexture(obj.text, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	// Import Normal map
	GLuint normal = AddTexture(obj.norm, GL_TEXTURE2);
	shaderPtr->CheckUniformLocation("normalTex", 2);

	// Import Specc Glow map
	GLuint specular = AddTexture(obj.spec, GL_TEXTURE3);
	shaderPtr->CheckUniformLocation("specularTex", 3);

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh, shaderPtr);

	Model model = Model(mesh, texture, normal, specular, _modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color); // plus modelID o matrixPointer, active


	// Push back the model
	if (_modelToLoad->RenderType == RENDER_FORWARD)
		m_modelsForward.push_back(model);
	else if (_modelToLoad->RenderType == RENDER_VIEWSPACE)
		m_modelsViewspace.push_back(model);
	else if (_modelToLoad->RenderType == RENDER_INTERFACE)
		m_modelsInterface.push_back(model);
	else
		m_modelsForward.push_back(model);
}

void GraphicDevice::BufferModels()
{
	for (auto pair : m_modelsToLoad)
	{
		BufferModel(pair.first, pair.second);
		delete(pair.second);
	}
	m_modelsToLoad.clear();
}

struct sort_depth
{
	inline bool operator() (const Model& a, const Model& b)
	{
		return (*a.modelMatrix)[3][2] < (*b.modelMatrix)[3][2];
	}
};

void GraphicDevice::SortModelsBasedOnDepth(std::vector<Model>* models)
{
	std::sort(models->begin(), models->end(), sort_depth());
}