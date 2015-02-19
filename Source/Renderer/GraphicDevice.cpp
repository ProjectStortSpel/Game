#include "GraphicDevice.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "TextureLoader.h"
#include "ModelLoader.h"
#include "ModelExporter.h"

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
	m_particleID = 0;
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
	m_particleID = 0;
}
GraphicDevice::~GraphicDevice()
{
	delete(m_skybox);
	delete m_pointerToPointlights;

	for (std::map<int, ParticleSystem*>::iterator it = m_particleSystems.begin(); it != m_particleSystems.end(); ++it)
	{
		delete(it->second);
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
#pragma region Inits
bool GraphicDevice::InitSDLWindow(int _width, int _height)
{
	// WINDOW SETTINGS
	unsigned int	Flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	int				SizeX = _width;	//1280
	int				SizeY = _height;	//720
	if (SDL_Init(SDL_INIT_VIDEO) == -1){
		std::cout << SDL_GetError() << std::endl;
		return false;
	}

	// PLATFORM SPECIFIC CODE
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	m_window = SDL_CreateWindow(m_windowCaption, m_windowPosX, m_windowPosY, SizeX, SizeY, Flags);
	if (m_window == NULL){
		std::cout << SDL_GetError() << std::endl;
		return false;
	}
	SDL_GetWindowSize(m_window, &m_clientWidth, &m_clientHeight);
	return true;
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
#pragma endregion in the order they are initialized

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

void GraphicDevice::CreateParticleSystems()
{
	//glEnable(GL_POINT_SPRITE);
	//m_particleSystems[m_particleID] = (new ParticleSystem("fire", vec3(11.0f, 0.55f, 8.0f), 100, 700, 0.05f, 0.6f, AddTexture("content/textures/firewhite.png", GL_TEXTURE1), vec3(0.8f, 0.f, 0.0f), &m_particleShader));
	//m_particleID++;
	//m_particleSystems.push_back(new ParticleSystem("smoke", vec3(11.0f, 0.0f, 9.0f), 15, 1800, 0.05f, 1.6f, AddTexture("content/textures/smoke1.png", GL_TEXTURE1), vec3(0.5f, 0.f, 0.f), &m_particleShader));
	int tmpID;
	
	//AddParticleEffect("fire", vec3(11.0f, 0.55f, 8.0f), 100, 700, 0.05f, 0.6f, "content/textures/fire3.png", vec3(0.0f, 8.f, 0.0f), tmpID);

//	AddParticleEffect("fire", vec3(8.0f, 0.55f, 8.0f), 100, 1100, 0.05f, 0.6f, "content/textures/firewhite4.png", vec3(0.9f, 0.f, 0.0f), tmpID);
	//AddParticleEffect("fire", vec3(8.0f, 0.55f, 5.0f), 100, 700, 0.05f, 0.6f, "content/textures/smoke1.png", vec3(0.2f, 0.f, 1.0f), tmpID);
	//AddParticleEffect("smoke", vec3(11.0f, 0.0f, 9.0f), 15, 1800, 0.05f, 1.6f, "content/textures/smoke1.png", vec3(0.0f, 0.f, 0.f), tmpID);
}

void GraphicDevice::AddParticleEffect(std::string _name, const vec3 _pos, int _nParticles, float _lifeTime, float _scale, float _spriteSize, std::string _texture, vec3 _color, int &_id)
{
	ParticleSystemToLoad tmpSystem;
	tmpSystem.Name = _name;
	tmpSystem.Pos = _pos;
	tmpSystem.NrOfParticles = _nParticles;
	tmpSystem.LifeTime = _lifeTime;
	tmpSystem.Scale = _scale;
	tmpSystem.SpriteSize = _spriteSize;
	tmpSystem.TextureName = _texture;
	tmpSystem.Color = _color;
	tmpSystem.Id = m_particleID;

	m_particleSystemsToLoad.push_back(tmpSystem);

	_id = m_particleID; 
	m_particleID++;
}

void GraphicDevice::RemoveParticleEffect(int _id)
{
	m_particlesIdToRemove.push_back(_id);
}

void GraphicDevice::BufferParticleSystems()
{
	for (int i = 0; i < m_particlesIdToRemove.size(); i++)
	{
		delete(m_particleSystems[m_particlesIdToRemove[i]]);
		m_particleSystems.erase(m_particlesIdToRemove[i]);
	}
	m_particlesIdToRemove.clear();

	for (int i = 0; i < m_particleSystemsToLoad.size(); i++)
	{
		m_particleSystems.insert(std::pair<int, ParticleSystem*>(m_particleSystemsToLoad[i].Id,new ParticleSystem(
			m_particleSystemsToLoad[i].Name,
			m_particleSystemsToLoad[i].Pos,
			m_particleSystemsToLoad[i].NrOfParticles,
			m_particleSystemsToLoad[i].LifeTime,
			m_particleSystemsToLoad[i].Scale,
			m_particleSystemsToLoad[i].SpriteSize,
			AddTexture(m_particleSystemsToLoad[i].TextureName, GL_TEXTURE1),
			m_particleSystemsToLoad[i].Color,
			&m_particleShaders[m_particleSystemsToLoad[i].Name])));
	}
	m_particleSystemsToLoad.clear();

}

int GraphicDevice::LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType, float* _color)
{
	int modelID = m_modelIDcounter;
	m_modelIDcounter++;

	//	Lägg till i en lista, följande
	//	std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType

	ModelToLoad* modelToLoad = new ModelToLoad();
	modelToLoad->Dir = _dir;
	modelToLoad->File = _file;
	modelToLoad->MatrixPtr = _matrixPtr;
	modelToLoad->RenderType = _renderType;
	modelToLoad->Color = _color;
	m_modelsToLoad[modelID] = modelToLoad;

	return modelID;
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
void GraphicDevice::BufferModel(int _modelId, ModelToLoad* _modelToLoad)
{
	ObjectData obj = ModelLoader::importObject(_modelToLoad->Dir, _modelToLoad->File);
	Shader *shaderPtr = NULL;
	std::vector<Model> *modelList = NULL;

	//if (obj.animated)
	//{
	//	BufferAModel(_modelId, _modelToLoad);
	//	return;
	//}

	bool FoundShaderType = false;
	for (int i = 0; i < m_renderLists.size(); i++)
	{
		if (_modelToLoad->RenderType == m_renderLists[i].RenderType)
		{
			shaderPtr = m_renderLists[i].ShaderPtr;
			modelList = m_renderLists[i].ModelList;
			shaderPtr->UseProgram();
			FoundShaderType = true;
			break;
		}
	}
	if (!FoundShaderType) { ERRORMSG("ERROR: INVALID RENDER SETTING"); return; }

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh, shaderPtr, false);
	// Import Texture
	GLuint texture = AddTexture(obj.text, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	GLuint normal, specular;

	if (_modelToLoad->RenderType != RENDER_INTERFACE)
	{
		// Import Normal map
		normal = AddTexture(obj.norm, GL_TEXTURE2);
		shaderPtr->CheckUniformLocation("normalTex", 2);
		// Import Specc Glow map
		specular = AddTexture(obj.spec, GL_TEXTURE3);
		shaderPtr->CheckUniformLocation("specularTex", 3);
	}

	// Create model
	Model model;
	if (_modelToLoad->RenderType != RENDER_INTERFACE) // TODO: BETTER FIX
		model = Model(mesh, texture, normal, specular);
	else
		model = Model(mesh, texture, NULL, NULL);
	//for the matrices (modelView + normal)
	m_vramUsage += (16 + 9) * sizeof(float);


	for (int i = 0; i < modelList->size(); i++)
	{
		if ((*modelList)[i] == model)
		{
			(*modelList)[i].instances.push_back(Instance(_modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color));
			return;
		}
	}

	//if model doesnt exist
	model.instances.push_back(Instance(_modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color));
	// Push back the model
	modelList->push_back(model);
}
void GraphicDevice::BufferAModel(int _modelId, ModelToLoad* _modelToLoad)
{
	ObjectData obj = ModelLoader::importObject(_modelToLoad->Dir, _modelToLoad->File);

	Shader *shaderPtr = &m_animationShader;
	std::vector<AModel> *modelList = &m_modelsAnimated;

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh, shaderPtr, true);

	// Import Texture
	GLuint texture = AddTexture(obj.text, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	// Import Normal map
	GLuint normal = AddTexture(obj.norm, GL_TEXTURE2);
	shaderPtr->CheckUniformLocation("normalTex", 2);

	// Import Specc Glow map
	GLuint specular = AddTexture(obj.spec, GL_TEXTURE3);
	shaderPtr->CheckUniformLocation("specularTex", 3);

	// Import Skeleton
	std::vector<JointData> joints = ModelLoader::importJoints(obj.joints);

	AModel model = AModel(_modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color, mesh, texture, normal, specular);

	// Add skeleton
	for (int i = 0; i < joints.size(); i++)
	{
		model.joints.push_back(Joint(
		joints[i].x0, joints[i].y0, joints[i].z0, joints[i].w0,
		joints[i].x1, joints[i].y1, joints[i].z1, joints[i].w1,
		joints[i].x2, joints[i].y2, joints[i].z2, joints[i].w2,
		joints[i].x3, joints[i].y3, joints[i].z3, joints[i].parent)
		);//joints[i].transform));
	}
	glGenBuffers(1, &model.jointBuffer);

	//for the matrices (modelView + normal)
	m_vramUsage += (16 + 9) * sizeof(float);

	// Push back the model
	modelList->push_back(model);
}

bool GraphicDevice::RemoveModel(int _id)
{
	for (int k = 0; k < m_renderLists.size(); k++)
	{
		std::vector<Model> *modelList = m_renderLists[k].ModelList;
		for (int i = 0; i < (*modelList).size(); i++)
		{
			for (int j = 0; j < (*modelList)[i].instances.size(); j++)
			{
				if ((*modelList)[i].instances[j].id == _id)
				{
					(*modelList)[i].instances.erase((*modelList)[i].instances.begin() + j);
					if ((*modelList)[i].instances.size() == 0)
						(*modelList).erase((*modelList).begin() + i);

					return true;
				}
			}
		}
	}
	return false;
}
bool GraphicDevice::ActiveModel(int _id, bool _active)
{
	for (int k = 0; k < m_renderLists.size(); k++)
	{
		std::vector<Model> *modelList = m_renderLists[k].ModelList;
		for (int i = 0; i < (*modelList).size(); i++)
		{
			for (int j = 0; j < (*modelList)[i].instances.size(); j++)
			{
				if ((*modelList)[i].instances[j].id == _id)
				{
					(*modelList)[i].instances[j].active = _active;
					return true;
				}
			}
		}
	}
	return false;
}

Buffer* GraphicDevice::AddMesh(std::string _fileDir, Shader *_shaderProg, bool animated)
{
	for (std::map<const std::string, Buffer*>::iterator it = m_meshs.begin(); it != m_meshs.end(); it++)
	{
		if (it->first == _fileDir)
			return it->second;
	}

	ModelExporter modelExporter;
	modelExporter.OpenFileForRead(_fileDir.c_str());
	std::vector<float> positionData = modelExporter.ReadDataFromFile();
	std::vector<float> normalData = modelExporter.ReadDataFromFile();
	std::vector<float> tanData = modelExporter.ReadDataFromFile();
	std::vector<float> bitanData = modelExporter.ReadDataFromFile();
	std::vector<float> texCoordData = modelExporter.ReadDataFromFile();
	std::vector<float> jointIndexData = modelExporter.ReadDataFromFile();
	std::vector<float> jointWeightData = modelExporter.ReadDataFromFile();
	modelExporter.CloseFile();

	Buffer* retbuffer = new Buffer();

	_shaderProg->UseProgram();
	BufferData bufferData[] =
	{
		{ 0, 3, GL_FLOAT, (const GLvoid*)positionData.data(), static_cast<GLsizeiptr>(positionData.size() * sizeof(float)) },
		{ 1, 3, GL_FLOAT, (const GLvoid*)normalData.data(), static_cast<GLsizeiptr>(normalData.size()   * sizeof(float)) },
		{ 2, 3, GL_FLOAT, (const GLvoid*)tanData.data(), static_cast<GLsizeiptr>(tanData.size()   * sizeof(float)) },
		{ 3, 3, GL_FLOAT, (const GLvoid*)bitanData.data(), static_cast<GLsizeiptr>(bitanData.size()   * sizeof(float)) },
		{ 4, 2, GL_FLOAT, (const GLvoid*)texCoordData.data(), static_cast<GLsizeiptr>(texCoordData.size() * sizeof(float)) },
		{ 5, 4, GL_FLOAT, (const GLvoid*)jointIndexData.data(), static_cast<GLsizeiptr>(jointIndexData.size()   * sizeof(float)) },
		{ 6, 4, GL_FLOAT, (const GLvoid*)jointWeightData.data(), static_cast<GLsizeiptr>(jointWeightData.size() * sizeof(float)) }
	};

	int test = sizeof(bufferData) / sizeof(bufferData[0]);
	// Counts the size in bytes of all the buffered data
	for (int i = 0; i < sizeof(bufferData) / sizeof(bufferData[0]); i++)
		m_vramUsage += (int)bufferData[i].dataSize;

	int bufferDatas = sizeof(bufferData) / sizeof(bufferData[0]);
	if (animated == false)
		bufferDatas -= 2;
	else
	{
		for (int i = 0; i < jointIndexData.size(); i++)
		{
			if (jointIndexData[i] > 35 || jointIndexData[i] < 0)
			{
				int hej = 0;
			}
		}
	}

	retbuffer->init(bufferData, bufferDatas);
	retbuffer->setCount((int)positionData.size() / 3);

	m_meshs.insert(std::pair<const std::string, Buffer*>(_fileDir, retbuffer));

	return retbuffer;
}

bool GraphicDevice::BufferModelTexture(int _id, std::string _fileDir, int _textureType)
{
	// Model Instance
	Instance instance;
	// Temp Model
	Model model;

	bool found = false;
	int renderType;
	Shader *shaderPtr = NULL;
	std::vector<Model> *modelList = NULL;

	// Find model Instance
	for (int k = 0; k < m_renderLists.size(); k++)
	{
		modelList = m_renderLists[k].ModelList;
		for (int i = 0; i < (*modelList).size(); i++)
		{
			for (int j = 0; j < (*modelList)[i].instances.size(); j++)
			{
				if ((*modelList)[i].instances[j].id == _id)
				{
					instance = (*modelList)[i].instances[j];
					model = Model(
						(*modelList)[i].bufferPtr,
						(*modelList)[i].texID,
						(*modelList)[i].norID,
						(*modelList)[i].speID
						);
					found = true;
					renderType = m_renderLists[k].RenderType;
					shaderPtr = m_renderLists[k].ShaderPtr;
					(*modelList)[i].instances.erase((*modelList)[i].instances.begin() + j);
					if ((*modelList)[i].instances.size() == 0)
						(*modelList).erase((*modelList).begin() + i);
				}
				if (found) break;
			}
			if (found) break;
		}
		if (found) break;
	}

	// Didn't we find it return false
	if (!found) return false;

	// Set new Texture to TextureType
	if (_textureType == TEXTURE_DIFFUSE)
	{
		model.texID = GraphicDevice::AddTexture(_fileDir, GL_TEXTURE1);
		shaderPtr->CheckUniformLocation("diffuseTex", 1);
	}
	else if (_textureType == TEXTURE_NORMAL)
	{
		model.norID = GraphicDevice::AddTexture(_fileDir, GL_TEXTURE2);
		shaderPtr->CheckUniformLocation("normalTex", 2);
	}
	else if (_textureType == TEXTURE_SPECULAR)
	{
		model.speID = GraphicDevice::AddTexture(_fileDir, GL_TEXTURE3);
		shaderPtr->CheckUniformLocation("specularTex", 3);
	}

	// Check if our new Model type already exists and add instance
	for (int i = 0; i < (*modelList).size(); i++)
	{
		if ((*modelList)[i] == model)
		{
			(*modelList)[i].instances.push_back(instance);
			return true;
		}
	}

	// Nothing found. Let's make a new Model type
	model.instances.push_back(instance);
	// Push back the model
	(*modelList).push_back(model);

	return true;
}

void GraphicDevice::UpdateTextureIndex(GLuint newTexture, GLuint oldTexture)
{
	for (int k = 0; k < m_renderLists.size(); k++)
	{
		std::vector<Model> *modelList = m_renderLists[k].ModelList;
		for (Model& m : (*modelList))
			if (m.texID == oldTexture)
				m.texID = newTexture;
	}
}
