/* ANDROID VERSION */
#include "GraphicsHigh.h"

#include "ModelExporter.h"

using namespace Renderer;
using namespace glm;

GraphicsHigh::GraphicsHigh()
{
	m_modelIDcounter = 0;
}

GraphicsHigh::GraphicsHigh(Camera _camera) : GraphicDevice(_camera)
{
	m_modelIDcounter = 0;
	for (int i = 0; i < 10; i++)
		m_lightDefaults[i] = 0.0f;
}

GraphicsHigh::~GraphicsHigh()
{
	delete m_shadowMap;
	// Delete buffers
	for (std::map<const std::string, Buffer*>::iterator it = m_meshs.begin(); it != m_meshs.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	
	for (std::map<const std::string, GLuint>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		glDeleteTextures(1, &it->second);
	}

}

bool GraphicsHigh::Init()
{
	if (!InitSDLWindow()) { ERRORMSG("INIT SDL WINDOW FAILED\n"); return false; }

	if (!m_SDLinitialized)
		m_camera = new Camera(m_clientWidth, m_clientHeight);

	if (!InitShaders()) { ERRORMSG("INIT SHADERS FAILED\n"); return false; }
	if (!InitBuffers()) { ERRORMSG("INIT BUFFERS FAILED\n"); return false; }
	if (!InitSkybox()) { ERRORMSG("INIT SKYBOX FAILED\n"); return false; }

	CreateShadowMap();
	if (!InitLightBuffers()) { ERRORMSG("INIT LIGHTBUFFER FAILED\n"); return false; }
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.2f, 0.6f, 1.0f);

    TextRenderer::Init();
    
	return true;
}

void GraphicsHigh::Update(float _dt)
{
	m_camera->Update(_dt);

	BufferModels();
	BufferLightsToGPU();
	BufferSurfaces();
	BufferModelTextures();
}

void GraphicsHigh::WriteShadowMapDepth()
{
    GLint oldFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
    
	//------- Write shadow maps depths ----------
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMap->GetShadowFBOHandle());
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glViewport(0, 0, m_shadowMap->GetResolution()-2, m_shadowMap->GetResolution()-2);

	m_shadowShader.UseProgram();
	//------FORWARD RENDERING--------------------------------------------
	glEnable(GL_BLEND);

	//glCullFace(GL_FRONT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	mat4 shadowViewProj = (*m_shadowMap->GetProjectionMatrix()) * (*m_shadowMap->GetViewMatrix());

	//Forward models
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].active) // IS MODEL ACTIVE?
		{
			
			mat4 modelMatrix;
			if (m_modelsForward[i].modelMatrix == NULL)
			{
				modelMatrix = glm::translate(glm::vec3(1));
				SDL_Log("model: %d has no model matrix", i);
			}
			else
				modelMatrix = *m_modelsForward[i].modelMatrix;

			mat4 mvp = shadowViewProj * modelMatrix;
			m_shadowShader.SetUniVariable("MVP", mat4x4, &mvp);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].texID);

			m_modelsForward[i].bufferPtr->draw(m_shadowShader.GetShaderProgram());
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	//------------------------------------------------
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
	glCullFace(GL_BACK);
	//------------------------------
}

void GraphicsHigh::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//--------Uniforms-------------------------------------------------------------------------
	mat4 projectionMatrix = *m_camera->GetProjMatrix();
	mat4 viewMatrix = *m_camera->GetViewMatrix();

	if (m_modelsForward.size() > 0)
	{
		WriteShadowMapDepth();

		glViewport(0, 0, m_clientWidth, m_clientHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//------FORWARD RENDERING--------------------------------------------
		//glEnable(GL_BLEND);

		m_forwardShader.UseProgram();
		m_forwardShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
		m_forwardShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);
		glm::mat4 invViewMatrix = glm::inverse(viewMatrix);
		m_forwardShader.SetUniVariable("InvViewMatrix", mat4x4, &invViewMatrix);

		mat4 shadowVP = (*m_shadowMap->GetProjectionMatrix()) * (*m_shadowMap->GetViewMatrix());
		m_forwardShader.SetUniVariable("ShadowViewProj", mat4x4, &shadowVP);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_shadowMap->GetDepthTexHandle());

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

				m_forwardShader.SetUniVariable("BlendColor", vector3, m_modelsForward[i].color);

				m_modelsForward[i].bufferPtr->draw(m_forwardShader.GetShaderProgram());
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		//-------------------------------------------------------------------------
	}
	glViewport(0, 0, m_clientWidth, m_clientHeight);
	glDisable(GL_CULL_FACE);
	// DRAW SKYBOX
	m_skyBoxShader.UseProgram();
	m_skybox->Draw(m_skyBoxShader.GetShaderProgram(), m_camera);
	// -----------

	glEnable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	// RENDER VIEWSPACE STUFF
	m_viewspaceShader.UseProgram();
	m_viewspaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	SortModelsBasedOnDepth(&m_modelsViewspace);
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
			m_viewspaceShader.SetUniVariable("ModelViewMatrix", mat4x4, &modelViewMatrix);

			mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
			m_viewspaceShader.SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].texID);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].norID);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].speID);

			m_modelsViewspace[i].bufferPtr->draw(m_viewspaceShader.GetShaderProgram());
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	// RENDER INTERFACE
	m_interfaceShader.UseProgram();
	m_interfaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	SortModelsBasedOnDepth(&m_modelsInterface);
	for (int i = 0; i < m_modelsInterface.size(); i++)
	{
		if (m_modelsInterface[i].active) // IS MODEL ACTIVE?
		{
			mat4 modelMatrix;
			if (m_modelsInterface[i].modelMatrix == NULL)
				modelMatrix = glm::translate(glm::vec3(1));
			else
				modelMatrix = *m_modelsInterface[i].modelMatrix;

			mat4 modelViewMatrix = modelMatrix;
			m_interfaceShader.SetUniVariable("ModelViewMatrix", mat4x4, &modelViewMatrix);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_modelsInterface[i].texID);

			m_modelsInterface[i].bufferPtr->draw(m_interfaceShader.GetShaderProgram());
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	/*if (m_modelsForward.size() > 0)
	{
		float positionData[] = {
			-1.0, -1.0,
			1.0, -1.0,
			1.0, 1.0,
			1.0, 1.0,
			-1.0, 1.0,
			-1.0, -1.0
		};

		m_fullscreen.UseProgram();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_shadowMap->GetDepthTexHandle());

		GLuint buf;
		glGenBuffers(1, &buf);

		glBindBuffer(GL_ARRAY_BUFFER, buf);
		glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), positionData, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDeleteBuffers(1, &buf);
	}*/
	glDisable(GL_TEXTURE_2D);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);

	SDL_GL_SwapWindow(m_window);
}

bool GraphicsHigh::InitSDLWindow()
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
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	m_window = SDL_CreateWindow(Caption, PosX, PosY, SizeX, SizeY, Flags);

	if (m_window == NULL){
		SDL_Log( SDL_GetError() );
		return false;
	}

	SDL_GetWindowSize(m_window, &m_clientWidth, &m_clientHeight);
	SDL_Log("W: %d H: %d", m_clientWidth, m_clientHeight);

	m_glContext = SDL_GL_CreateContext(m_window);

	SDL_GL_SetSwapInterval(0);

	return true;
}

bool GraphicsHigh::InitShaders()
{
	// Standard forward
	m_forwardShader.InitShaderProgram();
	m_forwardShader.AddShader("content/shaders/android/AndroidForwardVS.glsl", GL_VERTEX_SHADER);
	m_forwardShader.AddShader("content/shaders/android/AndroidForwardFS.glsl", GL_FRAGMENT_SHADER);
	m_forwardShader.FinalizeShaderProgram();

	// SkyBox
	m_skyBoxShader.InitShaderProgram();
	m_skyBoxShader.AddShader("content/shaders/android/AndroidSkyboxShaderVS.glsl", GL_VERTEX_SHADER);
	m_skyBoxShader.AddShader("content/shaders/android/AndroidSkyboxShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_skyBoxShader.FinalizeShaderProgram();

	// Viewspace shader
	m_viewspaceShader.InitShaderProgram();
	m_viewspaceShader.AddShader("content/shaders/android/AndroidViewspaceShaderVS.glsl", GL_VERTEX_SHADER);
	m_viewspaceShader.AddShader("content/shaders/android/AndroidViewspaceShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_viewspaceShader.FinalizeShaderProgram();

	// Interface shader
	m_interfaceShader.InitShaderProgram();
	m_interfaceShader.AddShader("content/shaders/android/AndroidInterfaceVS.glsl", GL_VERTEX_SHADER);
	m_interfaceShader.AddShader("content/shaders/android/AndroidInterfaceFS.glsl", GL_FRAGMENT_SHADER);
	m_interfaceShader.FinalizeShaderProgram();

	// ShadowShader geometry
	m_shadowShader.InitShaderProgram();
	m_shadowShader.AddShader("content/shaders/android/AndroidShadowShaderVS.glsl", GL_VERTEX_SHADER);
	m_shadowShader.AddShader("content/shaders/android/AndroidShadowShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_shadowShader.FinalizeShaderProgram();

	//m_fullscreen
	/*m_fullscreen.InitShaderProgram();
	m_fullscreen.AddShader("content/shaders/android/AndroidFullscreenVS.glsl", GL_VERTEX_SHADER);
	m_fullscreen.AddShader("content/shaders/android/AndroidFullscreenFS.glsl", GL_FRAGMENT_SHADER);
	m_fullscreen.FinalizeShaderProgram();*/

	return true;
}

bool GraphicsHigh::InitBuffers()
{
	//Skybox shader
	m_skyBoxShader.CheckUniformLocation("cubemap", 1);

	//Fullscreen shader
	//m_fullscreen.CheckUniformLocation("sampler", 0);

	return true;
}

bool GraphicsHigh::InitLightBuffers()
{
	for (int i = 0; i < 10; i++)
		m_lightDefaults[i] = 0.0f;		//init 0.0

	float **tmpPtr = new float*[1];
	BufferPointlights(0, tmpPtr);
	delete tmpPtr;

	BufferDirectionalLight(&m_lightDefaults[0]);

	return true;
}

void GraphicsHigh::BufferPointlights(int _nrOfLights, float **_lightPointers)
{
	if (m_pointlightsPtr)
	{
		delete m_pointlightsPtr;
		m_pointlightsPtr = 0;
	}
		
	if (_nrOfLights == 0)
	{
		m_pointlightsPtr = new float*[1];
		m_pointlightsPtr[0] = &m_lightDefaults[0];
		m_nrOfLightsToBuffer = 0;
	}
	else if (_nrOfLights >= 1)
	{
		m_pointlightsPtr = _lightPointers;
		m_nrOfLightsToBuffer = std::min(_nrOfLights, 3);
	}
}

void GraphicsHigh::BufferDirectionalLight(float *_lightPointer)
{
	m_directionalLightPtr = _lightPointer ? _lightPointer : m_lightDefaults;
}

void GraphicsHigh::CreateShadowMap()
{
	int resolution = 1024;
	m_dirLightDirection = vec3(0.0f, -1.0f, 1.0f);
	vec3 midMap = vec3(8.0f, 0.0f, 8.0f);
	vec3 lightPos = midMap - (10.0f*normalize(m_dirLightDirection));
	m_shadowMap = new ShadowMap(lightPos, lightPos + normalize(m_dirLightDirection), resolution);
	m_shadowMap->CreateShadowMapTexture(GL_TEXTURE0);

	m_forwardShader.UseProgram();
	m_forwardShader.SetUniVariable("BiasMatrix", mat4x4, m_shadowMap->GetBiasMatrix());
	m_forwardShader.CheckUniformLocation("ShadowDepthTex", 0);

	m_shadowShader.CheckUniformLocation("diffuseTex", 1);
}

bool GraphicsHigh::PreLoadModel(std::string _dir, std::string _file, int _renderType)
{
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
	else if (_renderType == RENDER_INTERFACE)
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
int GraphicsHigh::LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType, float* _color)
{
	int modelID = m_modelIDcounter;
	m_modelIDcounter++;
	
	ModelToLoad* modelToLoad = new ModelToLoad();
	modelToLoad->Dir = _dir;
	modelToLoad->File = _file;
	modelToLoad->MatrixPtr = _matrixPtr;
	modelToLoad->RenderType = _renderType;
	modelToLoad->Color = _color;
	m_modelsToLoad[modelID] = modelToLoad;

	return modelID;
}

bool GraphicsHigh::RemoveModel(int _id)
{
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].id == _id)
		{
			m_modelsForward.erase(m_modelsForward.begin() + i);

			return true;
		}
	}
	for (int i = 0; i < m_modelsViewspace.size(); i++)
	{
		if (m_modelsViewspace[i].id == _id)
		{
			m_modelsViewspace.erase(m_modelsViewspace.begin() + i);

			return true;
		}
	}
	for (int i = 0; i < m_modelsInterface.size(); i++)
	{
		if (m_modelsInterface[i].id == _id)
		{
			m_modelsInterface.erase(m_modelsInterface.begin() + i);

			return true;
		}
	}

	return false;
}
bool GraphicsHigh::ActiveModel(int _id, bool _active)
{
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].id == _id)
		{
			m_modelsForward[i].active = _active;
			return true;
		}
	}
	for (int i = 0; i < m_modelsViewspace.size(); i++)
	{
		if (m_modelsViewspace[i].id == _id)
		{
			m_modelsViewspace[i].active = _active;
			return true;
		}
	}
	for (int i = 0; i < m_modelsInterface.size(); i++)
	{
		if (m_modelsInterface[i].id == _id)
		{
			m_modelsInterface[i].active = _active;
			return true;
		}
	}
	return false;
}

Buffer* GraphicsHigh::AddMesh(std::string _fileDir, Shader *_shaderProg)
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
	modelExporter.CloseFile();

	Buffer* retbuffer = new Buffer();

	std::map<GLuint, GLuint> vpLocs, vnLocs, tanLocs, bitanLocs, tcLocs;
	vpLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_shadowShader.GetShaderProgram()]	 = glGetAttribLocation(m_shadowShader.GetShaderProgram(), "VertexPosition");

	vnLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_shadowShader.GetShaderProgram()]	 = glGetAttribLocation(m_shadowShader.GetShaderProgram(), "VertexNormal");

	tanLocs[m_forwardShader.GetShaderProgram()]	  = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_shadowShader.GetShaderProgram()]	  = glGetAttribLocation(m_shadowShader.GetShaderProgram(), "VertexTangent");

	bitanLocs[m_forwardShader.GetShaderProgram()]	= glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_shadowShader.GetShaderProgram()]	= glGetAttribLocation(m_shadowShader.GetShaderProgram(), "VertexBiTangent");

	tcLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_shadowShader.GetShaderProgram()]	 = glGetAttribLocation(m_shadowShader.GetShaderProgram(), "VertexTexCoord");

	_shaderProg->UseProgram();
	BufferData bufferData[] =
	{
		{ vpLocs,	 3, GL_FLOAT, (const GLvoid*)positionData.data(), (GLsizeiptr)(positionData.size() * sizeof(float)) },
		{ vnLocs,	 3, GL_FLOAT, (const GLvoid*)normalData.data(), (GLsizeiptr)(normalData.size()   * sizeof(float)) },
		{ tanLocs,	 3, GL_FLOAT, (const GLvoid*)tanData.data(), (GLsizeiptr)(tanData.size()   * sizeof(float)) },
		{ bitanLocs, 3, GL_FLOAT, (const GLvoid*)bitanData.data(), (GLsizeiptr)(bitanData.size()   * sizeof(float)) },
		{ tcLocs,	 2, GL_FLOAT, (const GLvoid*)texCoordData.data(), (GLsizeiptr)(texCoordData.size() * sizeof(float)) }
	};

	retbuffer->init(bufferData, sizeof(bufferData) / sizeof(bufferData[0]), _shaderProg->GetShaderProgram());
	retbuffer->setCount((int)positionData.size() / 3);
	
	m_meshs.insert(std::pair<const std::string, Buffer*>(_fileDir, retbuffer));

	return retbuffer;
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

void GraphicsHigh::Clear()
{
  m_modelIDcounter = 0;
  
  m_modelsForward.clear();
  m_modelsViewspace.clear();
  m_modelsInterface.clear();

  float **tmpPtr = new float*[1];
  BufferPointlights(0, tmpPtr);
  delete tmpPtr;
  
  if (m_pointlightsPtr)
	  delete m_pointlightsPtr;

  m_pointlightsPtr = NULL;
  m_directionalLightPtr = NULL;
}

void GraphicsHigh::BufferLightsToGPU()
{
	if (m_directionalLightPtr)
	{
	    m_dirLightDirection = vec3(m_directionalLightPtr[0], m_directionalLightPtr[1], m_directionalLightPtr[2]);
	    vec3 intens = vec3(m_directionalLightPtr[3], m_directionalLightPtr[4], m_directionalLightPtr[5]);
	    vec3 color = vec3(m_directionalLightPtr[6], m_directionalLightPtr[7], m_directionalLightPtr[8]);

	    m_forwardShader.SetUniVariable("dirlightDirection", vector3, &m_dirLightDirection);
	    m_forwardShader.SetUniVariable("dirlightIntensity", vector3, &intens);
	    m_forwardShader.SetUniVariable("dirlightColor", vector3, &color);

		m_shadowMap->UpdateViewMatrix(vec3(8.0f, 0.0f, 8.0f) - (10.0f*normalize(m_dirLightDirection)), vec3(8.0f, 0.0f, 8.0f));
	}
	else
	{
		m_forwardShader.SetUniVariable("dirlightIntensity", vector3, &m_lightDefaults[0]);
		m_forwardShader.SetUniVariable("dirlightColor", vector3, &m_lightDefaults[0]);
	}
	
	// ------------Pointlights------------
	if (m_pointlightsPtr)
	{
		if (m_nrOfLightsToBuffer == 0)
		{
			for (int i = 0; i < 1; i++)
			{
				std::stringstream ss;
				ss << "pointlights[" << i << "].Position";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[0]);		ss.str(std::string());

				ss << "pointlights[" << i << "].Intensity";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[3]);		ss.str(std::string());

				ss << "pointlights[" << i << "].Color";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[6]);		ss.str(std::string());

				ss << "pointlights[" << i << "].Range";
				m_forwardShader.SetUniVariable(ss.str().c_str(), glfloat, &m_lightDefaults[9]);		ss.str(std::string());
			}
		}
		else
		{
			for (int i = 0; i < 1; i++)
			{
				std::stringstream ss;
				ss << "pointlights[" << i << "].Position";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][0]);		ss.str(std::string());

				ss << "pointlights[" << i << "].Intensity";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][3]);		ss.str(std::string());

				ss << "pointlights[" << i << "].Color";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][6]);		ss.str(std::string());

				ss << "pointlights[" << i << "].Range";
				m_forwardShader.SetUniVariable(ss.str().c_str(), glfloat, &m_pointlightsPtr[i][9]);		ss.str(std::string());
			}
		}
		delete m_pointlightsPtr;
		m_pointlightsPtr = 0;
	}
}