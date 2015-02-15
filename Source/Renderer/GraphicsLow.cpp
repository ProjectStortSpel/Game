#include "GraphicsLow.h"

#include "ModelLoader.h"
#include "ModelExporter.h"
#include <sstream>

using namespace Renderer;
using namespace glm;

GraphicsLow::GraphicsLow()
{
	m_modelIDcounter = 0;
	m_vramUsage = 0;
	m_debugTexFlag = 0;
	m_nrOfLightsToBuffer = 0;
    m_pointerToDirectionalLights = 0;
	m_pointerToPointlights = 0;
}

GraphicsLow::GraphicsLow(Camera _camera, int x, int y) : GraphicDevice(_camera, x, y)
{
	m_modelIDcounter = 0;
	m_vramUsage = 0;
	m_debugTexFlag = 0;
	m_nrOfLightsToBuffer = 0;
	m_pointerToDirectionalLights = 0;
	m_pointerToPointlights = 0;
}

GraphicsLow::~GraphicsLow()
{
	delete(m_camera);
	delete(m_shadowMap);
	// Delete buffers
	for (std::map<const std::string, Buffer*>::iterator it = m_meshs.begin(); it != m_meshs.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
}

bool GraphicsLow::Init()
{
	if (!InitSDLWindow()) { ERRORMSG("INIT SDL WINDOW FAILED\n"); return false; }

	if (!m_SDLinitialized)
		m_camera = new Camera(m_clientWidth, m_clientHeight);

	if (!InitGLEW()) { ERRORMSG("GLEW_VERSION_4_0 FAILED.\n INCOMPATIBLE GRAPHICS DRIVER\n"); }
	if (!InitShaders()) { ERRORMSG("INIT SHADERS FAILED\n"); return false; }
	if (!InitBuffers()) { ERRORMSG("INIT BUFFERS FAILED\n"); return false; }
	if (!InitSkybox()) { ERRORMSG("INIT SKYBOX FAILED\n"); return false; }
	
	CreateShadowMap();
	if (!InitLightBuffers()) { ERRORMSG("INIT LIGHTBUFFER FAILED\n"); return false; }

	CreateParticleSystems();
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	TextRenderer::Init();
    
	return true;
}


void GraphicsLow::Update(float _dt)
{
	m_dt = _dt; m_fps = 1 / _dt;

	m_camera->Update(_dt);

	BufferModels();
	BufferLightsToGPU();
	BufferSurfaces();
	BufferModelTextures();
	BufferParticleSystems();
}

void GraphicsLow::WriteShadowMapDepth()
{
	//------- Write shadow maps depths ----------
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMap->GetShadowFBOHandle());
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_shadowMap->GetResolution(), m_shadowMap->GetResolution());

	//glCullFace(GL_FRONT);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(4.5, 18000.0);

	mat4 shadowProjection = *m_shadowMap->GetProjectionMatrix();

	//------Forward------------------------------------
	m_shadowShaderForward.UseProgram();
	//Forward models
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		std::vector<mat4> MVPVector(m_modelsForward[i].instances.size());
		std::vector<mat3> normalMatVector(m_modelsForward[i].instances.size());

		int nrOfInstances = 0;

		for (int j = 0; j < m_modelsForward[i].instances.size(); j++)
		{
			if (m_modelsForward[i].instances[j].active) // IS MODEL ACTIVE?
			{
				mat4 modelMatrix;
				if (m_modelsForward[i].instances[j].modelMatrix == NULL)
					modelMatrix = glm::translate(glm::vec3(1));
				else
					modelMatrix = *m_modelsForward[i].instances[j].modelMatrix;

				mat4 mvp = shadowProjection * (*m_shadowMap->GetViewMatrix()) * modelMatrix;
				MVPVector[nrOfInstances] = mvp;

				nrOfInstances++;
			}
		}
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].texID);

		m_modelsForward[i].bufferPtr->drawInstanced(0, nrOfInstances, &MVPVector, &normalMatVector);
	}
	//------------------------------------------------

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
	//glDisable(GL_POLYGON_OFFSET_FILL);
	//------------------------------
}

void GraphicsLow::Render()
{
		//GLTimer glTimer;
		//glTimer.Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	WriteShadowMapDepth();

	glViewport(0, 0, m_clientWidth, m_clientHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//--------Uniforms-------------------------------------------------------------------------
	mat4 projectionMatrix = *m_camera->GetProjMatrix();

	mat4 viewMatrix = *m_camera->GetViewMatrix();

	mat4 inverseProjection = glm::inverse(projectionMatrix);

	mat4 shadowVP = (*m_shadowMap->GetProjectionMatrix()) * (*m_shadowMap->GetViewMatrix());

		//m_glTimerValues.push_back(GLTimerValue("Deferred stage2: ", glTimer.Stop()));
		//glTimer.Start();

	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_forwardFBO);

	glEnable(GL_DEPTH_TEST);
    
	// DRAW SKYBOX
	glDisable(GL_CULL_FACE);
	m_skyBoxShader.UseProgram();
	m_skybox->Draw(m_skyBoxShader.GetShaderProgram(), m_camera);
	glEnable(GL_CULL_FACE);
	// -----------
   
	//------FORWARD RENDERING--------------------------------------------
	glEnable(GL_BLEND);

	m_forwardShader.UseProgram();
	m_forwardShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
	m_forwardShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);

	m_forwardShader.SetUniVariable("ShadowViewProj", mat4x4, &shadowVP);

	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		std::vector<mat4> modelViewVector(m_modelsForward[i].instances.size());
		std::vector<mat3> normalMatVector(m_modelsForward[i].instances.size());

		int nrOfInstances = 0;

		for (int j = 0; j < m_modelsForward[i].instances.size(); j++)
		{
			if (m_modelsForward[i].instances[j].active) // IS MODEL ACTIVE?
			{
				mat4 modelMatrix;
				if (m_modelsForward[i].instances[j].modelMatrix == NULL)
					modelMatrix = glm::translate(glm::vec3(1));
				else
					modelMatrix = *m_modelsForward[i].instances[j].modelMatrix;

				mat4 modelViewMatrix;
				modelViewMatrix = viewMatrix * modelMatrix;

				modelViewVector[nrOfInstances] = modelViewMatrix;

				mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
				normalMatVector[nrOfInstances] = normalMatrix;

				m_forwardShader.SetUniVariable("BlendColor", vector3, m_modelsForward[i].instances[j].color);

				nrOfInstances++;
			}
		}

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].texID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].norID);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].speID);

		m_modelsForward[i].bufferPtr->drawInstanced(0, m_modelsForward[i].instances.size(), &modelViewVector, &normalMatVector);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	//------PARTICLES---------
	glEnable(GL_POINT_SPRITE);
	glDepthMask(GL_FALSE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	m_particleShader.UseProgram();

	m_particleShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
	//glUniformMatrix4fv(glGetUniformLocation(particleShaderProgHandle, "ProjectionMatrix"), 1, GL_FALSE, &mCameraProjectionMat[0][0]);

	glActiveTexture(GL_TEXTURE1);

	for (std::map<int, ParticleSystem*>::iterator it = m_particleSystems.begin(); it != m_particleSystems.end(); ++it)
	{
		glBindTexture(GL_TEXTURE_2D, it->second->GetTexHandle());

		mat4 Model = glm::translate(it->second->GetWorldPos());
		mat4 ModelView = viewMatrix * Model;

		m_particleShader.SetUniVariable("ModelView", mat4x4, &ModelView);
		m_particleShader.SetUniVariable("BlendColor", vector3, it->second->GetColor());

		it->second->Render(m_dt);
	}
	glDisable(GL_POINT_SPRITE);
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//------------------------

	// RENDER VIEWSPACE STUFF
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	m_viewspaceShader.UseProgram();
	m_viewspaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	SortModelsBasedOnDepth(&m_modelsViewspace);
	for (int i = 0; i < m_modelsViewspace.size(); i++)
	{
		std::vector<mat4> modelViewVector(m_modelsViewspace[i].instances.size());
		std::vector<mat3> normalMatVector(m_modelsViewspace[i].instances.size());

		int nrOfInstances = 0;

		for (int j = 0; j < m_modelsViewspace[i].instances.size(); j++)
		{
			if (m_modelsViewspace[i].instances[j].active) // IS MODEL ACTIVE?
			{
				mat4 modelMatrix;
				if (m_modelsViewspace[i].instances[j].modelMatrix == NULL)
					modelMatrix = glm::translate(glm::vec3(1));
				else
					modelMatrix = *m_modelsViewspace[i].instances[j].modelMatrix;

				mat4 modelViewMatrix;
				modelViewMatrix = modelMatrix;

				modelViewVector[nrOfInstances] = modelViewMatrix;

				mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
				normalMatVector[nrOfInstances] = normalMatrix;

				nrOfInstances++;
			}
		}

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].texID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].norID);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].speID);

		m_modelsViewspace[i].bufferPtr->drawInstanced(0, m_modelsViewspace[i].instances.size(), &modelViewVector, &normalMatVector);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// RENDER INTERFACE STUFF
	//glDisable(GL_DEPTH_TEST);
	m_interfaceShader.UseProgram();
	m_interfaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	SortModelsBasedOnDepth(&m_modelsInterface);
	for (int i = 0; i < m_modelsInterface.size(); i++)
	{
		std::vector<mat4> modelViewVector(m_modelsInterface[i].instances.size());
		std::vector<mat3> normalMatVector(m_modelsInterface[i].instances.size());

		int nrOfInstances = 0;

		for (int j = 0; j < m_modelsInterface[i].instances.size(); j++)
		{
			if (m_modelsInterface[i].instances[j].active) // IS MODEL ACTIVE?
			{
				mat4 modelMatrix;
				if (m_modelsInterface[i].instances[j].modelMatrix == NULL)
					modelMatrix = glm::translate(glm::vec3(1));
				else
					modelMatrix = *m_modelsInterface[i].instances[j].modelMatrix;

				mat4 modelViewMatrix = modelMatrix;

				modelViewVector[nrOfInstances] = modelViewMatrix;

				mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
				normalMatVector[nrOfInstances] = normalMatrix;

				nrOfInstances++;
			}
		}

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_modelsInterface[i].texID);

		m_modelsInterface[i].bufferPtr->drawInstanced(0, m_modelsInterface[i].instances.size(), &modelViewVector, &normalMatVector);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//---------------------------------------------------------------------
	

	//	//m_glTimerValues.push_back(GLTimerValue("Text Render: ", glTimer.Stop()));
	//	//glTimer.Start();

	// FULL SCREEN QUAD
	//m_fullScreenShader.UseProgram();
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, m_shadowMap->GetDepthTexHandle());
	//glDrawArrays(GL_POINTS, 0, 1);

	glUseProgram(0);
		//m_glTimerValues.push_back(GLTimerValue("RENDER: ", glTimer.Stop()));

	// Swap in the new buffer
	SDL_GL_SwapWindow(m_window);
}

bool GraphicsLow::InitSDLWindow()
{
	// WINDOW SETTINGS
	unsigned int	Flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	int				PosX = 2;
	int				PosY = 2;

	int				SizeX = 256 * 5;	//1280
	int				SizeY = 144 * 5;	//720

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
		std::cout << SDL_GetError() << std::endl;
		return false;
	}

	// PLATFORM SPECIFIC CODE
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    //Erik Mac
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
    //SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	m_window = SDL_CreateWindow(m_windowCaption, m_windowPosX, m_windowPosY, SizeX, SizeY, Flags);

	if (m_window == NULL){
		std::cout << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GetWindowSize(m_window, &m_clientWidth, &m_clientHeight);

	return true;
}

bool GraphicsLow::InitGLEW()
{
	m_glContext = SDL_GL_CreateContext(m_window);

    glewExperimental = GL_TRUE;
	if (glewInit() != 0) return false;
    int low, high;
    
    glGetIntegerv(GL_MINOR_VERSION, &low);
    glGetIntegerv(GL_MAJOR_VERSION, &high);
    
    char* version = (char*)glGetString(GL_VERSION);
    
#ifdef WIN32
	if (!GLEW_VERSION_4_0) { return false; }
#else
	if (!glewIsSupported("GL_VERSION_4_0")) { return false; }
#endif

	SDL_GL_SetSwapInterval(0);
	
    
	return true;
}

bool GraphicsLow::InitShaders()
{
	// SkyBox
	m_skyBoxShader.InitShaderProgram();
	m_skyBoxShader.AddShader("content/shaders/skyboxShaderVS.glsl", GL_VERTEX_SHADER);
	m_skyBoxShader.AddShader("content/shaders/skyboxShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_skyBoxShader.FinalizeShaderProgram();

	// Forward shader
	m_forwardShader.InitShaderProgram();
	m_forwardShader.AddShader("content/shaders/lowVSForwardShader.glsl", GL_VERTEX_SHADER);
	m_forwardShader.AddShader("content/shaders/lowFSForwardShader.glsl", GL_FRAGMENT_SHADER);
	m_forwardShader.FinalizeShaderProgram();

	// Viewspace shader
	m_viewspaceShader.InitShaderProgram();
	m_viewspaceShader.AddShader("content/shaders/VSViewspaceShader.glsl", GL_VERTEX_SHADER);
	m_viewspaceShader.AddShader("content/shaders/FSViewspaceShader.glsl", GL_FRAGMENT_SHADER);
	m_viewspaceShader.FinalizeShaderProgram();

	// Interface shader
	m_interfaceShader.InitShaderProgram();
	m_interfaceShader.AddShader("content/shaders/VSInterfaceShader.glsl", GL_VERTEX_SHADER);
	m_interfaceShader.AddShader("content/shaders/FSInterfaceShader.glsl", GL_FRAGMENT_SHADER);
	m_interfaceShader.FinalizeShaderProgram();

	// ShadowShader forward geometry
	m_shadowShaderForward.InitShaderProgram();
	m_shadowShaderForward.AddShader("content/shaders/shadowShaderForwardVS.glsl", GL_VERTEX_SHADER);
	m_shadowShaderForward.AddShader("content/shaders/shadowShaderForwardFS.glsl", GL_FRAGMENT_SHADER);
	m_shadowShaderForward.FinalizeShaderProgram();

	// Particle shader
	m_particleShader.InitShaderProgram();
	m_particleShader.AddShader("content/shaders/particleShaderVS.glsl", GL_VERTEX_SHADER);
	m_particleShader.AddShader("content/shaders/particleShaderFS.glsl", GL_FRAGMENT_SHADER);
	const char * outputNames[] = { "Position", "Velocity", "StartTime" };
	glTransformFeedbackVaryings(m_particleShader.GetShaderProgram(), 3, outputNames, GL_SEPARATE_ATTRIBS);
	m_particleShader.FinalizeShaderProgram();

	return true;
}

bool GraphicsLow::InitBuffers()
{
	//Forward shader
	m_forwardShader.CheckUniformLocation("diffuseTex", 1);

	//Skybox shader
	m_skyBoxShader.CheckUniformLocation("cubemap", 1);

	//Shadow forward shader
	m_shadowShaderForward.CheckUniformLocation("diffuseTex", 1);

	//Particle shader
	m_particleShader.CheckUniformLocation("ParticleTex", 1);

	return true;
}

bool GraphicsLow::InitLightBuffers()
{
	for (int i = 0; i < 10; i++)
		m_lightDefaults[i] = 0.0;		//init 0.0

	float **tmpPtr = new float*[1];
	BufferPointlights(0, tmpPtr);
	delete tmpPtr;

	BufferDirectionalLight(&m_lightDefaults[0]);
	
	return true;
}

void GraphicsLow::BufferPointlights(int _nrOfLights, float **_lightPointers)
{
	if (m_pointerToPointlights)
	{
		delete m_pointerToPointlights;
		m_pointerToPointlights = 0;
	}
		

	if (_nrOfLights == 0)
	{
		_nrOfLights = 3;
		m_pointerToPointlights = new float*[1];
		m_pointerToPointlights[0] = &m_lightDefaults[0];
		m_numberOfPointlights = 1;
	}
	else if (_nrOfLights >= 1)
	{
		m_pointerToPointlights = _lightPointers;
	}
	m_nrOfLightsToBuffer = _nrOfLights;
}


void GraphicsLow::BufferDirectionalLight(float *_lightPointer)
{
	if (_lightPointer == 0)
		m_pointerToDirectionalLights = &m_lightDefaults[0];
	else
		m_pointerToDirectionalLights = _lightPointer;
}


void GraphicsLow::BufferLightsToGPU()
{
	//lägg in pointlights här också
	if (m_pointerToDirectionalLights)
	{
		m_dirLightDirection = vec3(m_pointerToDirectionalLights[0], m_pointerToDirectionalLights[1], m_pointerToDirectionalLights[2]);
		vec3 intens = vec3(m_pointerToDirectionalLights[3], m_pointerToDirectionalLights[4], m_pointerToDirectionalLights[5]);
		vec3 color = vec3(m_pointerToDirectionalLights[6], m_pointerToDirectionalLights[7], m_pointerToDirectionalLights[8]);

		m_forwardShader.SetUniVariable("dirlightDirection", vector3, &m_dirLightDirection);
		m_forwardShader.SetUniVariable("dirlightIntensity", vector3, &intens);
		m_forwardShader.SetUniVariable("dirlightColor", vector3, &color);

		m_shadowMap->UpdateViewMatrix(vec3(8.0f, 0.0f, 8.0f) - (10.0f*normalize(m_dirLightDirection)), vec3(8.0f, 0.0f, 8.0f));
	}
	else
	{
		vec3 zero = vec3(0.0f);
		m_forwardShader.SetUniVariable("dirlightIntensity", vector3, &zero);
		m_forwardShader.SetUniVariable("dirlightColor", vector3, &zero);
	}

	// ------------Pointlights------------
	if (m_pointerToPointlights && m_numberOfPointlights == 1 && m_nrOfLightsToBuffer == 3)
	{
		for (int i = 0; i < 3; i++)
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
		delete m_pointerToPointlights;
		m_pointerToPointlights = 0;
		m_nrOfLightsToBuffer = 0;
	}
	else if(m_pointerToPointlights)
	{
		int nrOfLights;
		if (m_nrOfLightsToBuffer > 3)
			nrOfLights = 3;
		else
			nrOfLights = m_nrOfLightsToBuffer;

		for (int i = 0; i < nrOfLights; i++)
		{
			std::stringstream ss;
			ss << "pointlights[" << i << "].Position";
			m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointerToPointlights[i][0]);		ss.str(std::string());

			ss << "pointlights[" << i << "].Intensity";
			m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointerToPointlights[i][3]);		ss.str(std::string());

			ss << "pointlights[" << i << "].Color";
			m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointerToPointlights[i][6]);		ss.str(std::string());

			ss << "pointlights[" << i << "].Range";
			m_forwardShader.SetUniVariable(ss.str().c_str(), glfloat, &m_pointerToPointlights[i][9]);		ss.str(std::string());

		}
		delete m_pointerToPointlights;
		m_pointerToPointlights = 0;
		m_nrOfLightsToBuffer = 0;
	}

}

void GraphicsLow::CreateShadowMap()
{
	int resolution = 1024;
	m_dirLightDirection = vec3(0.0, -1.0, 1.0);
	vec3 midMap = vec3(8.0, 0.0, 8.0);
	vec3 lightPos = midMap - (10.0f*normalize(m_dirLightDirection));
	m_shadowMap = new ShadowMap(lightPos, lightPos + normalize(m_dirLightDirection), resolution);
	m_shadowMap->CreateShadowMapTexture(GL_TEXTURE10);

	m_forwardShader.UseProgram();
	m_forwardShader.SetUniVariable("BiasMatrix", mat4x4, m_shadowMap->GetBiasMatrix());
	m_forwardShader.CheckUniformLocation("ShadowDepthTex", 10);

	m_vramUsage += (resolution*resolution*sizeof(float));
}

bool GraphicsLow::PreLoadModel(std::string _dir, std::string _file, int _renderType)
{
	Shader *shaderPtr = NULL;
	if (_renderType == RENDER_DEFERRED)
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
	}
	else if (_renderType == RENDER_FORWARD)
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
		shaderPtr = &m_viewspaceShader;
		m_interfaceShader.UseProgram();
	}
	else
		ERRORMSG("ERROR: INVALID RENDER SETTING");

	// Import Object
	ObjectData obj = ModelLoader::importObject(_dir, _file);

	// Import Texture
	GLuint texture = GraphicDevice::AddTexture(obj.text, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	if (_renderType != RENDER_INTERFACE)
	{
		// Import Normal map
		GLuint normal = GraphicDevice::AddTexture(obj.norm, GL_TEXTURE2);
		shaderPtr->CheckUniformLocation("normalTex", 2);

		// Import Specc Glow map
		GLuint specular = GraphicDevice::AddTexture(obj.spec, GL_TEXTURE3);
		shaderPtr->CheckUniformLocation("specularTex", 3);
	}

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh, shaderPtr);

	return true;
}

int GraphicsLow::LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType, float* _color)
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
void GraphicsLow::BufferModel(int _modelId, ModelToLoad* _modelToLoad)
{
	Shader *shaderPtr = NULL;
	if (_modelToLoad->RenderType == RENDER_FORWARD || _modelToLoad->RenderType == RENDER_DEFERRED)
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
		ERRORMSG("ERROR: INVALID RENDER SETTING");

	// Import Object
	//ObjectData obj = AddObject(_dir, _file);
	ObjectData obj = ModelLoader::importObject(_modelToLoad->Dir, _modelToLoad->File);

	// Import Texture
	GLuint texture = GraphicDevice::AddTexture(obj.text, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	// Import Normal map
	GLuint normal = GraphicDevice::AddTexture(obj.norm, GL_TEXTURE2);
	shaderPtr->CheckUniformLocation("normalTex", 2);

	// Import Specc Glow map
	GLuint specular = GraphicDevice::AddTexture(obj.spec, GL_TEXTURE3);
	shaderPtr->CheckUniformLocation("specularTex", 3);

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh, shaderPtr);

	Model model = Model(mesh, texture, normal, specular);

	//for the matrices (modelView + normal)
	m_vramUsage += (16 + 9) * sizeof(float);

	if (_modelToLoad->RenderType == RENDER_FORWARD || _modelToLoad->RenderType == RENDER_DEFERRED)
	{
		for (int i = 0; i < m_modelsForward.size(); i++)
		{
			if (m_modelsForward[i] == model)
			{
				m_modelsForward[i].instances.push_back(Instance(_modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color));
				return;
			}
		}
	}
	else if (_modelToLoad->RenderType == RENDER_VIEWSPACE)
	{
		for (int i = 0; i < m_modelsViewspace.size(); i++)
		{
			if (m_modelsViewspace[i] == model)
			{
				m_modelsViewspace[i].instances.push_back(Instance(_modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color));
				return;
			}
		}
	}
	else if (_modelToLoad->RenderType == RENDER_INTERFACE)
	{
		for (int i = 0; i < m_modelsInterface.size(); i++)
		{
			if (m_modelsInterface[i] == model)
			{
				m_modelsInterface[i].instances.push_back(Instance(_modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color));
				return;
			}
		}
	}

	// Set model
	//if model doesnt exist
	model.instances.push_back(Instance(_modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color));
	// Push back the model
	if (_modelToLoad->RenderType == RENDER_FORWARD || _modelToLoad->RenderType == RENDER_DEFERRED)
		m_modelsForward.push_back(model);
	else if (_modelToLoad->RenderType == RENDER_VIEWSPACE)
		m_modelsViewspace.push_back(model);
	else if (_modelToLoad->RenderType == RENDER_INTERFACE)
		m_modelsInterface.push_back(model);
}

void GraphicsLow::BufferModels()
{
	for (auto pair : m_modelsToLoad)
	{
		BufferModel(pair.first, pair.second);
		delete(pair.second);
	}
	m_modelsToLoad.clear();
}

bool GraphicsLow::RemoveModel(int _id)
{
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		for (int j = 0; j < m_modelsForward[i].instances.size(); j++)
		{
			if (m_modelsForward[i].instances[j].id == _id)
			{
				m_modelsForward[i].instances.erase(m_modelsForward[i].instances.begin() + j);
				if (m_modelsForward[i].instances.size() == 0)
					m_modelsForward.erase(m_modelsForward.begin() + i);

				return true;
			}
		}
	}
	for (int i = 0; i < m_modelsViewspace.size(); i++)
	{
		for (int j = 0; j < m_modelsViewspace[i].instances.size(); j++)
		{
			if (m_modelsViewspace[i].instances[j].id == _id)
			{
				m_modelsViewspace[i].instances.erase(m_modelsViewspace[i].instances.begin() + j);
				if (m_modelsViewspace[i].instances.size() == 0)
					m_modelsViewspace.erase(m_modelsViewspace.begin() + i);

				return true;
			}
		}
	}
	for (int i = 0; i < m_modelsInterface.size(); i++)
	{
		for (int j = 0; j < m_modelsInterface[i].instances.size(); j++)
		{
			if (m_modelsInterface[i].instances[j].id == _id)
			{
				m_modelsInterface[i].instances.erase(m_modelsInterface[i].instances.begin() + j);
				if (m_modelsInterface[i].instances.size() == 0)
					m_modelsInterface.erase(m_modelsInterface.begin() + i);

				return true;
			}
		}
	}

	return false;
}
bool GraphicsLow::ActiveModel(int _id, bool _active)
{
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		for (int j = 0; j < m_modelsForward[i].instances.size(); j++)
		{
			if (m_modelsForward[i].instances[j].id == _id)
			{
				m_modelsForward[i].instances[j].active = _active;
				return true;
			}
		}
	}

	for (int i = 0; i < m_modelsViewspace.size(); i++)
	{
		for (int j = 0; j < m_modelsViewspace[i].instances.size(); j++)
		{
			if (m_modelsViewspace[i].instances[j].id == _id)
			{
				m_modelsViewspace[i].instances[j].active = _active;
				return true;
			}
		}
	}

	for (int i = 0; i < m_modelsInterface.size(); i++)
	{
		for (int j = 0; j < m_modelsInterface[i].instances.size(); j++)
		{
			if (m_modelsInterface[i].instances[j].id == _id)
			{
				m_modelsInterface[i].instances[j].active = _active;
				return true;
			}
		}
	}
	return false;
}

Buffer* GraphicsLow::AddMesh(std::string _fileDir, Shader *_shaderProg)
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

	_shaderProg->UseProgram();
	BufferData bufferData[] =
	{
		{ 0, 3, GL_FLOAT, (const GLvoid*)positionData.data(), static_cast<GLsizeiptr>(positionData.size() * sizeof(float)) },
		{ 1, 3, GL_FLOAT, (const GLvoid*)normalData.data(), static_cast<GLsizeiptr>(normalData.size()   * sizeof(float)) },
		{ 2, 3, GL_FLOAT, (const GLvoid*)tanData.data(), static_cast<GLsizeiptr>(tanData.size()   * sizeof(float)) },
		{ 3, 3, GL_FLOAT, (const GLvoid*)bitanData.data(), static_cast<GLsizeiptr>(bitanData.size()   * sizeof(float)) },
		{ 4, 2, GL_FLOAT, (const GLvoid*)texCoordData.data(), static_cast<GLsizeiptr>(texCoordData.size() * sizeof(float)) }
	};

	int test = sizeof(bufferData) / sizeof(bufferData[0]);
	// Counts the size in bytes of all the buffered data
	for (int i = 0; i < sizeof(bufferData) / sizeof(bufferData[0]); i++)
		m_vramUsage += (int)bufferData[i].dataSize;

	retbuffer->init(bufferData, sizeof(bufferData) / sizeof(bufferData[0]));
	retbuffer->setCount((int)positionData.size() / 3);
	
	m_meshs.insert(std::pair<const std::string, Buffer*>(_fileDir, retbuffer));

	return retbuffer;
}

void GraphicsLow::Clear()
{
	m_modelIDcounter = 0;
	
	m_modelsForward.clear();
	m_modelsViewspace.clear();
	m_modelsInterface.clear();

	BufferPointlights(0, 0);
	BufferDirectionalLight(0);

	if (m_pointerToPointlights)
		delete m_pointerToPointlights;
	
	m_pointerToPointlights = NULL;
	m_pointerToDirectionalLights = NULL;
	m_numberOfPointlights = 0;
	m_numberOfDirectionalLights = 0;
}

bool GraphicsLow::BufferModelTexture(int _id, std::string _fileDir, int _textureType)
{
	// TODO: Do this for Interface and Viewspace
	// Model Instance
	Instance instance;
	// Temp Model
	Model model;

	bool found = false;
	int renderType;

	// Find model Instance
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		for (int j = 0; j < m_modelsForward[i].instances.size(); j++)
		{
			if (m_modelsForward[i].instances[j].id == _id)
			{
				instance = m_modelsForward[i].instances[j];
				model = Model(
					m_modelsForward[i].bufferPtr,
					m_modelsForward[i].texID,
					m_modelsForward[i].norID,
					m_modelsForward[i].speID
					);
				found = true;
				renderType = RENDER_FORWARD;
				m_modelsForward[i].instances.erase(m_modelsForward[i].instances.begin() + j);
				if (m_modelsForward[i].instances.size() == 0)
					m_modelsForward.erase(m_modelsForward.begin() + i);
			}
			if (found) break;
		}
		if (found) break;
	}
	if (!found)
	{
		for (int i = 0; i < m_modelsViewspace.size(); i++)
		{
			for (int j = 0; j < m_modelsViewspace[i].instances.size(); j++)
			{
				if (m_modelsViewspace[i].instances[j].id == _id)
				{
					instance = m_modelsViewspace[i].instances[j];
					model = Model(
						m_modelsViewspace[i].bufferPtr,
						m_modelsViewspace[i].texID,
						m_modelsViewspace[i].norID,
						m_modelsViewspace[i].speID
						);
					found = true;
					renderType = RENDER_VIEWSPACE;
					m_modelsViewspace[i].instances.erase(m_modelsViewspace[i].instances.begin() + j);
					if (m_modelsViewspace[i].instances.size() == 0)
						m_modelsViewspace.erase(m_modelsViewspace.begin() + i);
				}
				if (found) break;
			}
			if (found) break;
		}
	}
	if (!found)
	{
		for (int i = 0; i < m_modelsInterface.size(); i++)
		{
			for (int j = 0; j < m_modelsInterface[i].instances.size(); j++)
			{
				if (m_modelsInterface[i].instances[j].id == _id)
				{
					instance = m_modelsInterface[i].instances[j];
					model = Model(
						m_modelsInterface[i].bufferPtr,
						m_modelsInterface[i].texID,
						m_modelsInterface[i].norID,
						m_modelsInterface[i].speID
						);
					found = true;
					renderType = RENDER_INTERFACE;
					m_modelsInterface[i].instances.erase(m_modelsInterface[i].instances.begin() + j);
					if (m_modelsInterface[i].instances.size() == 0)
						m_modelsInterface.erase(m_modelsInterface.begin() + i);
				}
				if (found) break;
			}
			if (found) break;
		}
	}
	// Didn't we find it return false
	if (!found) return false;

	// Set new Texture to TextureType
	if (_textureType == TEXTURE_DIFFUSE)
	{
		model.texID = GraphicDevice::AddTexture(_fileDir, GL_TEXTURE1);
	}
	else if (_textureType == TEXTURE_NORMAL)
	{
		model.norID = GraphicDevice::AddTexture(_fileDir, GL_TEXTURE2);
	}
	else if (_textureType == TEXTURE_SPECULAR)
	{
		model.speID = GraphicDevice::AddTexture(_fileDir, GL_TEXTURE3);
	}

	// Check if our new Model type already exists and add instance
	if (renderType == RENDER_FORWARD)
	{
		for (int i = 0; i < m_modelsForward.size(); i++)
		{
			if (m_modelsForward[i] == model)
			{
				m_modelsForward[i].instances.push_back(instance);
				return true;
			}
		}
	}
	else if (renderType == RENDER_VIEWSPACE)
	{
		for (int i = 0; i < m_modelsViewspace.size(); i++)
		{
			if (m_modelsViewspace[i] == model)
			{
				m_modelsViewspace[i].instances.push_back(instance);
				return true;
			}
		}
	}
	else if (renderType == RENDER_INTERFACE)
	{
		for (int i = 0; i < m_modelsInterface.size(); i++)
		{
			if (m_modelsInterface[i] == model)
			{
				m_modelsInterface[i].instances.push_back(instance);
				return true;
			}
		}
	}

	// Nothing found. Let's make a new Model type
	model.instances.push_back(instance);
	// Push back the model
	if (renderType == RENDER_FORWARD)
		m_modelsForward.push_back(model);
	else if (renderType == RENDER_VIEWSPACE)
		m_modelsViewspace.push_back(model);
	else if (renderType == RENDER_INTERFACE)
		m_modelsInterface.push_back(model);

	return true;
}

void GraphicsLow::UpdateTextureIndex(GLuint newTexture, GLuint oldTexture)
{
	for (Model& m : m_modelsForward)
		if (m.texID == oldTexture)
			m.texID = newTexture;
	for (Model& m : m_modelsViewspace)
		if (m.texID == oldTexture)
			m.texID = newTexture;
	for (Model& m : m_modelsInterface)
		if (m.texID == oldTexture)
			m.texID = newTexture;
}
