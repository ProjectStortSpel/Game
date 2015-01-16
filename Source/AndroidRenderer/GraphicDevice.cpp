#include "GraphicDevice.h"

#define STB_IMAGE_IMPLEMENTATION
#include "TextureLoader.h"
#include "ModelLoader.h"
#include "ModelExporter.h"
#include "SkyBox.h"

using namespace Renderer;
using namespace glm;

GraphicDevice::GraphicDevice()
{
	m_modelIDcounter = 0;
	m_vramUsage = 0;
}

GraphicDevice::~GraphicDevice()
{
	delete(m_camera);
	delete(m_skybox);
	// Delete buffers
	for (std::map<const std::string, Buffer*>::iterator it = m_meshs.begin(); it != m_meshs.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}

	SDL_GL_DeleteContext(m_glContext);
	// Close and destroy the window
	SDL_DestroyWindow(m_window);
	// Clean up
	SDL_Quit();
}

bool GraphicDevice::Init()
{
	if (!InitSDLWindow()) { ERRORMSG("INIT SDL WINDOW FAILED\n"); return false; }

	m_camera = new Camera(m_clientWidth, m_clientHeight);

	if (!InitShaders()) { ERRORMSG("INIT SHADERS FAILED\n"); return false; }
	if (!InitBuffers()) { ERRORMSG("INIT BUFFERS FAILED\n"); return false; }
	//if (!InitSkybox()) { ERRORMSG("INIT SKYBOX FAILED\n"); return false; }
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.2f, 0.6f, 1.0f);

	return true;
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

void GraphicDevice::Update(float _dt)
{
  //SDL_Log("FPS: %f", 1.0f/_dt);
}

void GraphicDevice::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, m_clientWidth, m_clientHeight);
	
	//--------Uniforms-------------------------------------------------------------------------
	mat4 projectionMatrix = *m_camera->GetProjMatrix();

	mat4 viewMatrix = *m_camera->GetViewMatrix();

	// DRAW SKYBOX
	//m_skyBoxShader.UseProgram();
	//m_skybox->Draw(m_skyBoxShader.GetShaderProgram(), m_camera);
	// -----------

	vec3 tPos = *m_camera->GetPos() + 8.0f* (*m_camera->GetLook());

	//------FORWARD RENDERING--------------------------------------------
	//glEnable(GL_BLEND);

	m_forwardShader.UseProgram();
	m_forwardShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
	m_forwardShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);

	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].active) // IS MODEL ACTIVE?
		{
			mat4 modelMatrix;
			if (m_modelsForward[i].modelMatrix == NULL)
				modelMatrix = glm::translate(glm::vec3(1));
			else
				modelMatrix = *m_modelsForward[i].modelMatrix;

			mat4 modelViewMatrix = viewMatrix * modelMatrix;
			m_forwardShader.SetUniVariable("ModelViewMatrix", mat4x4, &modelViewMatrix);

			mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
			m_forwardShader.SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].texID);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].norID);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].speID);

			m_modelsForward[i].bufferPtr->draw();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glDisable(GL_DEPTH_TEST);

	// RENDER VIEWSPACE STUFF
	m_viewspaceShader.UseProgram();
	m_viewspaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	for (int i = 0; i < m_modelsViewspace.size(); i++)
	{
		if (m_modelsViewspace[i].active) // IS MODEL ACTIVE?
		{
			mat4 modelMatrix;
			if (m_modelsViewspace[i].modelMatrix == NULL)
				modelMatrix = glm::translate(glm::vec3(1));
			else
				modelMatrix = *m_modelsViewspace[i].modelMatrix;

			mat4 modelViewMatrix = modelMatrix;
			m_forwardShader.SetUniVariable("ModelViewMatrix", mat4x4, &modelViewMatrix);

			mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
			m_forwardShader.SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].texID);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].norID);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].speID);

			m_modelsViewspace[i].bufferPtr->draw();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);

	SDL_GL_SwapWindow(m_window);
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

bool GraphicDevice::InitSDLWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1){
		SDL_Log( SDL_GetError() );
		return false;
	}

	// WINDOW SETTINGS
	unsigned int	Flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	const char*		Caption = "SDL Window";
	int				PosX = 0;
	int				PosY = 0;

	SDL_DisplayMode dm;
	SDL_GetDisplayMode(0, 0, &dm);
	int				SizeX = dm.w;	//1280
	int				SizeY = dm.h;	//720

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	m_window = SDL_CreateWindow(Caption, PosX, PosY, SizeX, SizeY, Flags);

	if (m_window == NULL){
		SDL_Log( SDL_GetError() );
		return false;
	}

	SDL_GetWindowSize(m_window, &m_clientWidth, &m_clientHeight);

	m_glContext = SDL_GL_CreateContext(m_window);

	SDL_GL_SetSwapInterval(0);

	return true;
}

bool GraphicDevice::InitShaders()
{
	// Standard forward
	m_forwardShader.InitShaderProgram();
	m_forwardShader.AddShader("content/shaders/AndroidForwardVS.glsl", GL_VERTEX_SHADER);
	m_forwardShader.AddShader("content/shaders/AndroidForwardFS.glsl", GL_FRAGMENT_SHADER);
	m_forwardShader.FinalizeShaderProgram();

	// Viewspace shader
	m_viewspaceShader.InitShaderProgram();
	m_viewspaceShader.AddShader("content/shaders/AndroidViewspaceShaderVS.glsl", GL_VERTEX_SHADER);
	m_viewspaceShader.AddShader("content/shaders/AndroidViewspaceShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_viewspaceShader.FinalizeShaderProgram();

	return true;
}

bool GraphicDevice::InitBuffers()
{
	//Skybox shader
	//m_skyBoxShader.CheckUniformLocation("cubemap", 1);

	return true;
}

bool GraphicDevice::InitSkybox()
{
	/*int w, h;
	GLuint texHandle = TextureLoader::LoadCubeMap("content/textures/skybox", GL_TEXTURE1, w, h);
	if (texHandle < 0)
		return false;

	m_skyBoxShader.UseProgram();
	m_skybox = new SkyBox(texHandle, m_camera->GetFarPlane());
	m_vramUsage += (w*h * 6 * 4 * sizeof(float));*/

	return true;
}

void GraphicDevice::BufferPointlights(int _nrOfLights, float **_lightPointers)
{

}

void GraphicDevice::BufferDirectionalLight(float *_lightPointer)
{

}

bool GraphicDevice::RenderSimpleText(std::string _text, int _x, int _y)
{
	return true;
}
void GraphicDevice::SetSimpleTextColor(float _r, float _g, float _b, float _a)
{
}
void GraphicDevice::SetDisco()
{
}
void GraphicDevice::ToggleSimpleText(bool _render)
{
}
void GraphicDevice::ToggleSimpleText()
{
}

bool GraphicDevice::PreLoadModel(std::string _dir, std::string _file, int _renderType)
{
	Shader *shaderPtr = &m_forwardShader;
	m_forwardShader.UseProgram();

	// Import Object
	//ObjectData obj = AddObject(_dir, _file);
	ObjectData obj = ModelLoader::importObject(_dir, _file);

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

	return true;
}
int GraphicDevice::LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType)
{
	int modelID = m_modelIDcounter;
	m_modelIDcounter++;

	//Shader *shaderPtr = &m_forwardShader;
	//m_forwardShader.UseProgram();

	Shader *shaderPtr = NULL;

	if (_renderType == RENDER_FORWARD)
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
	}
	else if (_renderType == RENDER_VIEWSPACE)
	{
		shaderPtr = &m_viewspaceShader;
		m_viewspaceShader.UseProgram();
	}
	/*else if (_renderType == RENDER_INTERFACE)
	{
		shaderPtr = &m_interfaceShader;
		m_interfaceShader.UseProgram();
	}*/
	else
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
		SDL_Log("ERROR: INVALID RENDER SETTING. Selecting FORWARD");
	}

	// Import Object
	ObjectData obj = ModelLoader::importObject(_dir, _file);

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

	Model model = Model(mesh, texture, normal, specular, modelID, true, _matrixPtr); // plus modelID o matrixPointer, active


	// Push back the model
	if (_renderType == RENDER_FORWARD)
		m_modelsForward.push_back(model);
	else if (_renderType == RENDER_VIEWSPACE)
		m_modelsViewspace.push_back(model);
	else
		m_modelsForward.push_back(model);

	//else if (_renderType == RENDER_INTERFACE)
	//	m_modelsInterface.push_back(model);

	return modelID;
}
bool GraphicDevice::RemoveModel(int _id)
{
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		/*if (m_modelsForward[i].id == _id)
		{
			m_modelsForward[i].erase(m_modelsForward[i].begin() + i);

			return true;
		}*/
	}
	for (int i = 0; i < m_modelsViewspace.size(); i++)
	{
		if (m_modelsViewspace[i].id == _id)
		{
			m_modelsViewspace.erase(m_modelsViewspace.begin() + i);

			return true;
		}
	}

	return false;
}
bool GraphicDevice::ActiveModel(int _id, bool _active)
{
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		/*if (m_modelsForward[i].id == _id)
		{
			m_modelsForward[i].active = _active;
			return true;
		}*/
	}
	return false;
}
bool GraphicDevice::ChangeModelTexture(int _id, std::string _fileDir, int _textureType)
{
	//// Model Instance
	//Instance instance;
	//// Temp Model
	//Model model;

	//bool found = false;
	//int renderType;

	//// Find model Instance
	//for (int i = 0; i < m_modelsForward.size(); i++)
	//{
	//	for (int j = 0; j < m_modelsForward[i].instances.size(); j++)
	//	{
	//		if (m_modelsForward[i].instances[j].id == _id)
	//		{
	//			instance = m_modelsForward[i].instances[j];
	//			model = Model(
	//				m_modelsForward[i].bufferPtr,
	//				m_modelsForward[i].texID,
	//				m_modelsForward[i].norID,
	//				m_modelsForward[i].speID
	//				);
	//			found = true;
	//			renderType = RENDER_FORWARD;
	//			m_modelsForward[i].instances.erase(m_modelsForward[i].instances.begin() + j);
	//			if (m_modelsForward[i].instances.size() == 0)
	//				m_modelsForward.erase(m_modelsForward.begin() + i);
	//		}
	//		if (found) break;
	//	}
	//	if (found) break;
	//}
	//// Didn't we find it return false
	//if (!found) return false;

	//// Set new Texture to TextureType
	//if (_textureType == TEXTURE_DIFFUSE)
	//{
	//	model.texID = AddTexture(_fileDir, GL_TEXTURE1);
	//	m_forwardShader.CheckUniformLocation("diffuseTex", 1);
	//}
	//else if (_textureType == TEXTURE_NORMAL)
	//{
	//	model.norID = AddTexture(_fileDir, GL_TEXTURE2);
	//	m_forwardShader.CheckUniformLocation("normalTex", 2);
	//}
	//else if (_textureType == TEXTURE_SPECULAR)
	//{
	//	model.speID = AddTexture(_fileDir, GL_TEXTURE3);
	//	m_forwardShader.CheckUniformLocation("specularTex", 3);
	//}

	//// Check if our new Model type already exists and add instance
	//for (int i = 0; i < m_modelsForward.size(); i++)
	//{
	//	if (m_modelsForward[i] == model)
	//	{
	//		m_modelsForward[i].instances.push_back(instance);
	//		return true;
	//	}
	//}

	//// Nothing found. Let's make a new Model type
	//model.instances.push_back(instance);
	//// Push back the model
	//if (renderType == RENDER_DEFERRED)
	//	m_modelsDeferred.push_back(model);
	//else if (renderType == RENDER_FORWARD)
	//	m_modelsForward.push_back(model);

	return true;
}
bool GraphicDevice::ChangeModelNormalMap(int _id, std::string _fileDir)
{
	return ChangeModelTexture(_id, _fileDir, TEXTURE_NORMAL);
}
bool GraphicDevice::ChangeModelSpecularMap(int _id, std::string _fileDir)
{
	return ChangeModelTexture(_id, _fileDir, TEXTURE_SPECULAR);
}

Buffer* GraphicDevice::AddMesh(std::string _fileDir, Shader *_shaderProg)
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
	/*for (int i = 0; i < (int)texCoordData.size(); i++)
	{
	  if (texCoordData[i] < 0.0f)
	    texCoordData[i] = 0.0f;
	  else if (texCoordData[i] > 1.0f)
	    texCoordData[i] = 1.0f;
	}*/
	/*SDL_Log("File: %s", _fileDir.c_str());
	SDL_Log("texCoordData size: %d", texCoordData.size());
	for (int i = 0; i < 4; i++)
	  SDL_Log("texCoord[%d] = %f", i, texCoordData[i]);*/
	modelExporter.CloseFile();

	Buffer* retbuffer = new Buffer();

	GLuint vpLoc	= glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexPosition");
	GLuint vnLoc	= glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexNormal");
	GLuint tanLoc	= glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexTangent");
	GLuint bitanLoc = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexBiTangent");
	GLuint tcLoc	= glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexTexCoord");

	_shaderProg->UseProgram();
	BufferData bufferData[] =
	{
		{ vpLoc,	3, GL_FLOAT, (const GLvoid*)positionData.data(), (GLsizeiptr)(positionData.size() * sizeof(float)) },
		{ vnLoc,	3, GL_FLOAT, (const GLvoid*)normalData.data(), (GLsizeiptr)(normalData.size()   * sizeof(float)) },
		{ tanLoc,	3, GL_FLOAT, (const GLvoid*)tanData.data(), (GLsizeiptr)(tanData.size()   * sizeof(float)) },
		{ bitanLoc, 3, GL_FLOAT, (const GLvoid*)bitanData.data(), (GLsizeiptr)(bitanData.size()   * sizeof(float)) },
		{ tcLoc,	2, GL_FLOAT, (const GLvoid*)texCoordData.data(), (GLsizeiptr)(texCoordData.size() * sizeof(float)) }
	};

	int test = sizeof(bufferData) / sizeof(bufferData[0]);
	// Counts the size in bytes of all the buffered data
	for (int i = 0; i < sizeof(bufferData) / sizeof(bufferData[0]); i++)
		m_vramUsage += (int)bufferData[i].dataSize;

	retbuffer->init(bufferData, sizeof(bufferData) / sizeof(bufferData[0]), _shaderProg->GetShaderProgram());
	retbuffer->setCount((int)positionData.size() / 3);
	
	m_meshs.insert(std::pair<const std::string, Buffer*>(_fileDir, retbuffer));

	return retbuffer;
}
GLuint GraphicDevice::AddTexture(std::string _fileDir, GLenum _textureSlot)
{
	for (std::map<const std::string, GLuint>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		if (it->first == _fileDir)
			return it->second;
	}
	int texSizeX, texSizeY;
	//m_deferredShader1.UseProgram();
	GLuint texture = TextureLoader::LoadTexture(_fileDir.c_str(), _textureSlot, texSizeX, texSizeY);
	m_textures.insert(std::pair<const std::string, GLenum>(_fileDir, texture));
	m_vramUsage += (texSizeX * texSizeY * 4 * 4);
	return texture;
}
//ObjectData GraphicDevice::AddObject(std::string _file, std::string _dir)
//{
//	std::string fileDir = _dir;
//	fileDir.append(_file);
//	for (std::map<const std::string, ObjectData>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
//	{
//		if (it->first == fileDir)
//			return it->second;
//	}
//	ObjectData obj = ModelLoader::importObject(_dir, _file);
//}

void GraphicDevice::Clear()
{
  m_modelIDcounter = 0;
  
  m_modelsForward.clear();

  float **tmpPtr = new float*[1];
  BufferPointlights(0, tmpPtr);
  delete tmpPtr;
}
