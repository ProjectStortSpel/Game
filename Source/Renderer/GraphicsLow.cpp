#include "GraphicsLow.h"

//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif
//#include "TextureLoader.h"
//#include "TextureLoader.h"
#include "ModelLoader.h"
#include "ModelExporter.h"

using namespace Renderer;
using namespace glm;

GraphicsLow::GraphicsLow()
{
	m_modelIDcounter = 0;
	m_vramUsage = 0;
	m_debugTexFlag = 0;
	m_nrOfLights = 0;
}

GraphicsLow::~GraphicsLow()
{
	delete(m_camera);
	delete(m_skybox);
	delete(m_shadowMap);
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

bool GraphicsLow::Init()
{
	if (!InitSDLWindow()) { ERRORMSG("INIT SDL WINDOW FAILED\n"); return false; }

	m_camera = new Camera(m_clientWidth, m_clientHeight);

	if (!InitGLEW()) { SDL_Log("OGL 4.3 not supported. Switching to 4.0");/*ERRORMSG("GLEW_VERSION_4_3 FAILED\n");*/ }
	if (!InitShaders()) { ERRORMSG("INIT SHADERS FAILED\n"); return false; }
	if (!InitBuffers()) { ERRORMSG("INIT BUFFERS FAILED\n"); return false; }
	//if (!InitForward()) { ERRORMSG("INIT FORWARD FAILED\n"); return false; }
	if (!InitSkybox()) { ERRORMSG("INIT SKYBOX FAILED\n"); return false; }
	if (!InitRandomVector()) { ERRORMSG("INIT RANDOMVECTOR FAIELD\n"); return false; }
	
	CreateShadowMap();
	if (!InitLightBuffers()) { ERRORMSG("INIT LIGHTBUFFER FAILED\n"); return false; }
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	m_sdlTextRenderer.Init();
	
	return true;
}

//void GraphicsLow::PollEvent(SDL_Event _event)
//{
//	switch (_event.type)
//	{
//	case SDL_WINDOWEVENT:
//		switch (_event.window.event)
//		{
//		case SDL_WINDOWEVENT_RESIZED:
//			int w, h;
//			SDL_GetWindowSize(m_window, &w, &h);
//			ResizeWindow(w,h);
//			break;
//		}
//		break;
//
//	default:
//		break;
//	}
//}

void GraphicsLow::Update(float _dt)
{
	m_dt = _dt; m_fps = 1 / _dt;

	m_camera->Update(_dt);
}

void GraphicsLow::WriteShadowMapDepth()
{
	//------- Write shadow maps depths ----------
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMap->GetShadowFBOHandle());
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_shadowMap->GetResolution(), m_shadowMap->GetResolution());

	//glCullFace(GL_FRONT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(4.5, 18000.0);

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, 0);

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
	glDisable(GL_POLYGON_OFFSET_FILL);
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

	// RENDER VIEWSPACE STUFF
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	m_viewspaceShader.UseProgram();
	m_viewspaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

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
	glDisable(GL_DEPTH_TEST);
	m_interfaceShader.UseProgram();
	m_interfaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

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

//void GraphicsLow::ResizeWindow(int _width, int _height)
//{
//	// GRAPHIC CARD WORK GROUPS OF 16x16
//	int x, y;
//	x = _width / 16;
//	if (x == 0) x = 1;
//	y = _height / 16;
//	if (y == 0) y = 1;
//	m_clientWidth = x * 16;
//	m_clientHeight = y * 16;
//
//	std::cout << m_clientWidth << "x" << m_clientHeight << std::endl;
//
//	SDL_SetWindowSize(m_window, m_clientWidth, m_clientHeight);
//}

bool GraphicsLow::InitSDLWindow()
{
	// WINDOW SETTINGS
	unsigned int	Flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	const char*		Caption = "SDL Window";
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
    //SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	m_window = SDL_CreateWindow(Caption, PosX, PosY, SizeX, SizeY, Flags);

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

bool GraphicsLow::InitForward()
{
	//m_forwardShader.UseProgram();

	//// Create and bind the FBO
	//glGenFramebuffers(1, &m_forwardFBO);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_forwardFBO);

	//// Attach the images to the framebuffer
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuf, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_outputImage, 0);

	//GLenum drawBufferForward = GL_COLOR_ATTACHMENT0;
	//glDrawBuffers(1, &drawBufferForward);

	return true;
}

bool GraphicsLow::InitShaders()
{
	// SkyBox
	m_skyBoxShader.InitShaderProgram();
	m_skyBoxShader.AddShader("content/shaders/skyboxShaderVS.glsl", GL_VERTEX_SHADER);
	m_skyBoxShader.AddShader("content/shaders/skyboxShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_skyBoxShader.FinalizeShaderProgram();

	//// Full Screen Quad
	//m_fullScreenShader.InitShaderProgram();
	//m_fullScreenShader.AddShader("content/shaders/fullscreen.vs", GL_VERTEX_SHADER);
	//m_fullScreenShader.AddShader("content/shaders/fullscreen.gs", GL_GEOMETRY_SHADER);
	//m_fullScreenShader.AddShader("content/shaders/fullscreen.ps", GL_FRAGMENT_SHADER);
	//m_fullScreenShader.FinalizeShaderProgram();

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

	return true;
}

bool GraphicsLow::InitBuffers()
{
	// FULL SCREEN QUAD
	//m_fullScreenShader.CheckUniformLocation("output_image", 5);

	//Forward shader
	m_forwardShader.CheckUniformLocation("diffuseTex", 1);

	//Skybox shader
	m_skyBoxShader.CheckUniformLocation("cubemap", 1);

	//Shadow forward shader
	m_shadowShaderForward.CheckUniformLocation("diffuseTex", 1);

	return true;
}

//bool GraphicsLow::InitSkybox()
//{
//	int w, h;
//	GLuint texHandle = TextureLoader::LoadCubeMap("content/textures/skybox", GL_TEXTURE1, w, h);
//	if (texHandle < 0)
//		return false;
//
//	m_skyBoxShader.UseProgram();
//	m_skybox = new SkyBox(texHandle, m_camera->GetFarPlane());
//	m_vramUsage += (w*h * 6 * 4 * sizeof(float));
//
//	return true;
//}

bool GraphicsLow::InitRandomVector()
{
	int texSizeX, texSizeY;
	m_randomVectors = GraphicDevice::AddTexture("content/textures/vectormap.png", GL_TEXTURE21);

	return true;
}

bool GraphicsLow::InitLightBuffers()
{
	//glGenBuffers(1, &m_pointlightBuffer);

	//for (int i = 0; i < 10; i++)
	//	m_lightDefaults[i] = 0.0;		//init 0.0
	//
	//if (m_pointlightBuffer < 0)
	//	return false;

	//float ** tmparray = new float*[1];
	//tmparray[0] = &m_lightDefaults[0];

	//BufferPointlights(1, tmparray);

	//delete(tmparray);
	//
	//glGenBuffers(1, &m_dirLightBuffer);
	//if (m_dirLightBuffer < 0)
	//	return false;

	//BufferDirectionalLight(&m_lightDefaults[0]);
	
	return true;
}

void GraphicsLow::BufferPointlights(int _nrOfLights, float **_lightPointers)
{
	/*if (_nrOfLights == 0)
	{
	vec3 zero = vec3(0.0);
	m_forwardShader.SetUniVariable("pointlights[0].Position", vector3, &zero);
	m_forwardShader.SetUniVariable("pointlights[0].Intensity", vector3, &zero);
	m_forwardShader.SetUniVariable("pointlights[0].Color", vector3, &zero);
	m_forwardShader.SetUniVariable("pointlights[0].Range", glfloat, &zero.x);
	}
	else if (_nrOfLights >= 1)
	{
	m_forwardShader.SetUniVariable("pointlights[0].Position", vector3, &_lightPointers[0]);
	m_forwardShader.SetUniVariable("pointlights[0].Intensity", vector3, &_lightPointers[3]);
	m_forwardShader.SetUniVariable("pointlights[0].Color", vector3, &_lightPointers[6]);
	m_forwardShader.SetUniVariable("pointlights[0].Range", glfloat, &_lightPointers[9]);
	}*/
}

void GraphicsLow::BufferDirectionalLight(float *_lightPointer)
{
	//direction, intensity, color

	if (_lightPointer)
	{
		m_dirLightDirection = vec3(_lightPointer[0], _lightPointer[1], _lightPointer[2]);
		vec3 intens = vec3(_lightPointer[3], _lightPointer[4], _lightPointer[5]);
		vec3 color = vec3(_lightPointer[6], _lightPointer[7], _lightPointer[8]);

		m_forwardShader.SetUniVariable("dirlightDirection", vector3, &m_dirLightDirection);
		m_forwardShader.SetUniVariable("dirlightIntensity", vector3, &intens);
		m_forwardShader.SetUniVariable("dirlightColor", vector3, &color);
	}
	else
	{
		vec3 zero = vec3(0.0f);
		m_forwardShader.SetUniVariable("dirlightIntensity", vector3, &zero);
		m_forwardShader.SetUniVariable("dirlightColor", vector3, &zero);
	}

	m_shadowMap->UpdateViewMatrix(vec3(8.0f, 0.0f, 8.0f) - (10.0f*normalize(m_dirLightDirection)), vec3(8.0f, 0.0f, 8.0f));
}

void GraphicsLow::CreateShadowMap()
{
	int resolution = 2048;
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
	//ObjectData obj = AddObject(_dir, _file);
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
int GraphicsLow::LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType)
{
	int modelID = m_modelIDcounter;
	m_modelIDcounter++;

	Shader *shaderPtr = NULL;
	if (_renderType == RENDER_FORWARD || _renderType == RENDER_DEFERRED)
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
		ERRORMSG("ERROR: INVALID RENDER SETTING");

	// Import Object
	//ObjectData obj = AddObject(_dir, _file);
	ObjectData obj = ModelLoader::importObject(_dir, _file);

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

	if (_renderType == RENDER_FORWARD || _renderType == RENDER_DEFERRED)
	{
		for (int i = 0; i < m_modelsForward.size(); i++)
		{
			if (m_modelsForward[i] == model)
			{
				m_modelsForward[i].instances.push_back(Instance(modelID, true, _matrixPtr));
				return modelID;
			}
		}
	}
	else if (_renderType == RENDER_VIEWSPACE)
	{
		for (int i = 0; i < m_modelsViewspace.size(); i++)
		{
			if (m_modelsViewspace[i] == model)
			{
				m_modelsViewspace[i].instances.push_back(Instance(modelID, true, _matrixPtr));
				return modelID;
			}
		}
	}
	else if (_renderType == RENDER_INTERFACE)
	{
		for (int i = 0; i < m_modelsInterface.size(); i++)
		{
			if (m_modelsInterface[i] == model)
			{
				m_modelsInterface[i].instances.push_back(Instance(modelID, true, _matrixPtr));
				return modelID;
			}
		}
	}
	
	// Set model
	//if model doesnt exist
	model.instances.push_back(Instance(modelID, true, _matrixPtr));
	// Push back the model
	if (_renderType == RENDER_FORWARD || _renderType == RENDER_DEFERRED)
		m_modelsForward.push_back(model);
	else if (_renderType == RENDER_VIEWSPACE)
		m_modelsViewspace.push_back(model);
	else if (_renderType == RENDER_INTERFACE)
		m_modelsInterface.push_back(model);

	return modelID;
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
bool GraphicsLow::ChangeModelTexture(int _id, std::string _fileDir, int _textureType)
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
bool GraphicsLow::ChangeModelNormalMap(int _id, std::string _fileDir)
{
	// TODO: Do this for Interface and Viewspace
	return ChangeModelTexture(_id, _fileDir, TEXTURE_NORMAL);
}
bool GraphicsLow::ChangeModelSpecularMap(int _id, std::string _fileDir)
{
	// TODO: Do this for Interface and Viewspace
	return ChangeModelTexture(_id, _fileDir, TEXTURE_SPECULAR);
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
//GLuint GraphicsLow::AddTexture(std::string _fileDir, GLenum _textureSlot)
//{
//	for (std::map<const std::string, GLuint>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
//	{
//		if (it->first == _fileDir)
//			return it->second;
//	}
//	int texSizeX, texSizeY;
//	GLuint texture = TextureLoader::LoadTexture(_fileDir.c_str(), _textureSlot, texSizeX, texSizeY);
//	m_textures.insert(std::pair<const std::string, GLenum>(_fileDir, texture));
//	m_vramUsage += (texSizeX * texSizeY * 4 * 4);
//	return texture;
//}

void GraphicsLow::Clear()
{
  m_modelIDcounter = 0;
  
  m_modelsForward.clear();
  m_modelsViewspace.clear();
  m_modelsInterface.clear();

  float **tmpPtr = new float*[1];
  BufferPointlights(0, tmpPtr);
  delete tmpPtr;
}

//int GraphicsLow::AddFont(const std::string& filepath, int size)
//{
//	return m_sdlTextRenderer.AddFont(filepath, size);
//}
//
//float GraphicsLow::CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size)
//{
//	if (m_textures.find(textureName) != m_textures.end())
//		glDeleteTextures(1, &m_textures[textureName]);
//	SDL_Surface* surface = m_sdlTextRenderer.CreateTextSurface(textString, fontIndex, color);
//	if (size.x > 0)
//		surface->w = size.x;
//	if (size.y > 0)
//		surface->h = size.y;
//	//m_deferredShader1.UseProgram();
//	GLuint texture = TextureLoader::LoadTexture(surface, GL_TEXTURE1);
//	m_textures[textureName] = texture;
//	m_vramUsage += (surface->w * surface->h * 4 * 4);
//	SDL_FreeSurface(surface);
//	return (float)surface->w / (float)surface->h;
//}
//
//void GraphicsLow::CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size)
//{
//	if (m_textures.find(textureName) != m_textures.end())
//		glDeleteTextures(1, &m_textures[textureName]);
//	SDL_Surface* surface = m_sdlTextRenderer.CreateWrappedTextSurface(textString, fontIndex, color, wrapLength);
//	if (size.x > 0)
//		surface->w = size.x;
//	if (size.y > 0)
//		surface->h = size.y;
//	//m_deferredShader1.UseProgram();
//	GLuint texture = TextureLoader::LoadTexture(surface, GL_TEXTURE1);
//	m_textures[textureName] = texture;
//	m_vramUsage += (surface->w * surface->h * 4 * 4);
//	SDL_FreeSurface(surface);
//}
