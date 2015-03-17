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
#ifdef __ANDROID__
	if (m_clientWidth > 1400)
		InitFBO();
#endif
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
	m_dt = _dt;
	m_camera->Update(_dt);

	BufferModels();
	BufferLightsToGPU();
	BufferSurfaces();
	BufferModelTextures();
	BufferParticleSystems();
	
	for (int i = 0; i < m_modelsAnimated.size(); i++)
	{
		if (m_modelsAnimated[i].active) // IS MODEL ACTIVE?
		{
			m_modelsAnimated[i].Update(_dt);
		}
	}
}

void GraphicsHigh::WriteShadowMapDepth()
{
    //GLint oldFBO;
    //glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
    
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
		if (m_modelsForward[i].castShadow && m_modelsForward[i].active)
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
	//glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
	glCullFace(GL_BACK);
	//------------------------------
}

void GraphicsHigh::Render()
{
	// Cleara vanliga framebuffern också???
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

#if defined(__ANDROID__)
	GLint oldFBO = 0;
	if (m_clientWidth > 1400)
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
#elif defined(__IOS__)
	GLint oldFBO;
	glGetIntegerv(GL_FRAMEBUFFER, &oldFBO);
#endif


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);

	//--------Uniforms-------------------------------------------------------------------------
	mat4 projectionMatrix = *m_camera->GetProjMatrix();
	mat4 viewMatrix = *m_camera->GetViewMatrix();

	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	// DRAW SKYBOX
	m_skyBoxShader.UseProgram();
	glActiveTexture(GL_TEXTURE1);
	m_skybox->Draw(m_skyBoxShader.GetShaderProgram(), m_camera, m_dt);
	glEnable(GL_BLEND);

	m_skyboxCloudsShader.UseProgram();
	glActiveTexture(GL_TEXTURE2);
	m_skyboxClouds->Draw(m_skyBoxShader.GetShaderProgram(), m_camera, m_dt);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	// -----------

	if (m_modelsForward.size() > 0 || m_modelsAnimated.size() > 0)
	{
		WriteShadowMapDepth();

#if defined(__ANDROID__)
		if (m_clientWidth > 1400)
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		else
			glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
#elif defined(__IOS__)
		glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
#endif
		glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);

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
			m_modelsForward[i].Draw(viewMatrix, &m_forwardShader);
		}
		//-------------------------------------------------------------------------

		//-------Render water-------------
		m_riverShader.UseProgram();
		m_riverShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
		m_riverShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);
		m_riverShader.SetUniVariable("ShadowViewProj", mat4x4, &shadowVP);
		m_elapsedTime += m_dt;
		if (m_elapsedTime > 10.0f)
			m_elapsedTime = 0.0f;
		m_riverShader.SetUniVariable("ElapsedTime", glfloat, &m_elapsedTime);
		//----DRAW MODELS
		for (int i = 0; i < m_modelsWater.size(); i++)
		{
			m_modelsWater[i].Draw(viewMatrix, &m_riverShader);
		}

		//-------Render water corners-------------
		m_riverCornerShader.UseProgram();
		m_riverCornerShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
		m_riverCornerShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);
		m_riverCornerShader.SetUniVariable("ShadowViewProj", mat4x4, &shadowVP);
		m_riverCornerShader.SetUniVariable("ElapsedTime", glfloat, &m_elapsedTime);
		//----DRAW MODELS
		for (int i = 0; i < m_modelsWaterCorners.size(); i++)
		{
			m_modelsWaterCorners[i].Draw(viewMatrix, &m_riverCornerShader);
		}

		//--------ANIMATED DEFERRED RENDERING !!! ATTENTION: WORK IN PROGRESS !!!
		m_animationShader.UseProgram();
		m_animationShader.SetUniVariable("ShadowViewProj", mat4x4, &shadowVP);
		m_animationShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);
		for (int i = 0; i < m_modelsAnimated.size(); i++)
		{
			for (int j = 0; j < m_modelsAnimated[i].anim.size(); j++)
			{
				std::stringstream ss;
				ss << "anim[" << j << "]";
				m_animationShader.SetUniVariable(ss.str().c_str(), mat4x4, &m_modelsAnimated[i].anim[j]);
				ss.str(std::string());
			}

			m_modelsAnimated[i].Draw(viewMatrix, projectionMatrix, &m_animationShader);
		}
	}

	glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);



	//--------PARTICLES---------
	glDepthMask(GL_FALSE);

	glActiveTexture(GL_TEXTURE1);

	for (std::map<int, ParticleEffect*>::iterator it = m_particleEffects.begin(); it != m_particleEffects.end(); ++it)
	{
		Shader* thisShader = it->second->GetShaderPtr();
		thisShader->UseProgram();

		thisShader->SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
		glBindTexture(GL_TEXTURE_2D, it->second->GetTexHandle());

		mat4 Model = glm::translate(it->second->GetWorldPos());
		mat4 ModelView = viewMatrix * Model;

		thisShader->SetUniVariable("ModelView", mat4x4, &ModelView);
		thisShader->SetUniVariable("BlendColor", vector3, it->second->GetColor());

		it->second->Render(m_dt);
	}

	//glDisable(GL_POINT_SPRITE);
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//------------------------

	glClear(GL_DEPTH_BUFFER_BIT);

	// RENDER VIEWSPACE STUFF
	m_viewspaceShader.UseProgram();
	m_viewspaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	SortModelsBasedOnDepth(&m_modelsViewspace);
	for (int i = 0; i < m_modelsViewspace.size(); i++)
	{
		m_modelsViewspace[i].Draw(glm::mat4(1.0f), &m_viewspaceShader);
	}

	// RENDER INTERFACE
	m_interfaceShader.UseProgram();
	m_interfaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	SortModelsBasedOnDepth(&m_modelsInterface);
	for (int i = 0; i < m_modelsInterface.size(); i++)
	{
		if (m_modelsInterface[i].active)
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

			m_interfaceShader.SetUniVariable("BlendColor", vector3, m_modelsInterface[i].color);

			m_modelsInterface[i].bufferPtr->draw(m_interfaceShader.GetShaderProgram());
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);

	glDisable(GL_BLEND);
#ifdef __ANDROID__
	if (m_clientWidth > 1400)
	{
		// DRAW FULLSCREEN
		glViewport(0, 0, m_clientWidth, m_clientHeight);

		m_fullscreen.UseProgram();

		// Skicka in outputImage
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_outputImage);

		glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenQuadBuffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
#endif
    
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

#ifdef __IOS__
    Flags |= SDL_WINDOW_BORDERLESS;
    SDL_SetHint( "SDL_HINT_ORIENTATIONS", "LandscapeLeft LandscapeRight" );
#endif
    
	m_window = SDL_CreateWindow(Caption, PosX, PosY, SizeX, SizeY, Flags);

	if (m_window == NULL){
		SDL_Log( SDL_GetError() );
		return false;
	}

	SDL_GetWindowSize(m_window, &m_clientWidth, &m_clientHeight);
	SDL_Log("W: %d H: %d", m_clientWidth, m_clientHeight);

	m_framebufferWidth = m_clientWidth;
	m_framebufferHeight = m_clientHeight;

#ifdef __ANDROID__
	if (m_clientWidth > 1400)
	{
		m_framebufferWidth = 1280;
		m_framebufferHeight = m_framebufferWidth * float(float(m_clientHeight) / float(m_clientWidth));
	}
#endif

	m_glContext = SDL_GL_CreateContext(m_window);

	SDL_GL_SetSwapInterval(0);

	return true;
}

bool GraphicsHigh::InitShaders()
{
	InitStandardShaders();

	// Standard forward
	m_forwardShader.InitShaderProgram();
	m_forwardShader.AddShader("content/shaders/android/AndroidForwardVS.glsl", GL_VERTEX_SHADER);
	m_forwardShader.AddShader("content/shaders/android/AndroidForwardFS.glsl", GL_FRAGMENT_SHADER);
	m_forwardShader.FinalizeShaderProgram();

	// ShadowShader geometry
	m_shadowShader.InitShaderProgram();
	m_shadowShader.AddShader("content/shaders/android/AndroidShadowShaderVS.glsl", GL_VERTEX_SHADER);
	m_shadowShader.AddShader("content/shaders/android/AndroidShadowShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_shadowShader.FinalizeShaderProgram();

	// River water shader
	m_riverShader.InitShaderProgram();
	m_riverShader.AddShader("content/shaders/android/AndroidRiverShaderVS.glsl", GL_VERTEX_SHADER);
	m_riverShader.AddShader("content/shaders/android/AndroidForwardFS.glsl", GL_FRAGMENT_SHADER);
	m_riverShader.FinalizeShaderProgram();

	// River water corner shader
	m_riverCornerShader.InitShaderProgram();
	m_riverCornerShader.AddShader("content/shaders/android/AndroidForwardVS.glsl", GL_VERTEX_SHADER);
	m_riverCornerShader.AddShader("content/shaders/android/highAndroidRiverCornerFS.glsl", GL_FRAGMENT_SHADER);
	m_riverCornerShader.FinalizeShaderProgram();
	
	m_animationShader.InitShaderProgram();
	m_animationShader.AddShader("content/shaders/android/AndroidAnimationShaderVS.glsl", GL_VERTEX_SHADER);
	m_animationShader.AddShader("content/shaders/android/AndroidForwardFS.glsl", GL_FRAGMENT_SHADER);
	m_animationShader.FinalizeShaderProgram();

	return true;
}

bool GraphicsHigh::InitBuffers()
{
	InitStandardBuffers();

	return true;
}

bool GraphicsHigh::InitLightBuffers()
{
	for (int i = 0; i < 10; i++)
		m_lightDefaults[i] = 0.0f;		//init 0.0

	float **tmpPtr = new float*[1];
	BufferPointlights(0, tmpPtr);
	delete [] tmpPtr;

	BufferDirectionalLight(&m_lightDefaults[0]);

	return true;
}

void GraphicsHigh::BufferPointlights(int _nrOfLights, float **_lightPointers)
{
	if (m_pointlightsPtr)
	{
		delete [] m_pointlightsPtr;
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

	m_riverShader.UseProgram();
	m_riverShader.SetUniVariable("BiasMatrix", mat4x4, m_shadowMap->GetBiasMatrix());
	m_riverShader.CheckUniformLocation("ShadowDepthTex", 0);

	m_riverCornerShader.UseProgram();
	m_riverCornerShader.SetUniVariable("BiasMatrix", mat4x4, m_shadowMap->GetBiasMatrix());
	m_riverCornerShader.CheckUniformLocation("ShadowDepthTex", 0);

	m_shadowShader.CheckUniformLocation("diffuseTex", 1);
}

bool GraphicsHigh::PreLoadModel(std::vector<std::string> _dirs, std::string _file, int _renderType)
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
	ObjectData obj = ModelLoader::importObject(_dirs, _file);

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
int GraphicsHigh::LoadModel(std::vector<std::string> _dirs, std::string _file, glm::mat4 *_matrixPtr, int _renderType, float* _color, bool _castShadow, bool _isStatic)
{
	int modelID;

	if (!_isStatic)
	{
		modelID = m_modelIDcounter;
		m_modelIDcounter++;
	}
	
	ModelToLoad* modelToLoad = new ModelToLoad();
	modelToLoad->Dirs = _dirs;
	modelToLoad->File = _file;
	modelToLoad->MatrixPtr = _matrixPtr;
	modelToLoad->RenderType = _renderType;
	modelToLoad->Color = _color;
	modelToLoad->CastShadow = _castShadow;

	if (_isStatic)
	{
		for (std::map<int, std::vector<ModelToLoad*>>::iterator it = m_staticModelsToLoad.begin(); it != m_staticModelsToLoad.end(); it++)
		{
			if (!it->second.empty() &&
				it->second[0]->Dirs == _dirs &&
				it->second[0]->File == _file &&
				it->second[0]->RenderType == _renderType &&
				it->second[0]->CastShadow == _castShadow &&
					(it->second[0]->Color == _color ||
						(it->second[0]->Color[0] == _color[0] &&
						 it->second[0]->Color[1] == _color[1] &&
						 it->second[0]->Color[2] == _color[2])))
			{
				it->second.push_back(modelToLoad);
				return it->first;
			}
		}

		// If does not already exists
		modelID = m_modelIDcounter;
		m_modelIDcounter++;
		m_staticModelsToLoad[modelID].push_back(modelToLoad);
	}
	else
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
	for (int i = 0; i < m_modelsAnimated.size(); i++)
	{
		if (m_modelsAnimated[i].id == _id)
		{
			m_modelsAnimated.erase(m_modelsAnimated.begin() + i);
			return true;
		}
	}

	if (m_modelsToLoad.find(_id) != m_modelsToLoad.end())
	{
		m_modelsToLoad.erase(_id);

		return true;
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
	for (int i = 0; i < m_modelsAnimated.size(); i++)
	{
		if (m_modelsAnimated[i].id == _id)
		{
			m_modelsAnimated[i].active = _active;
			return true;
		}
	}
	return false;
}

Buffer* GraphicsHigh::AddMesh(std::string _fileDir, Shader *_shaderProg, bool animated)
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

	std::map<GLuint, GLuint> vpLocs, vnLocs, tanLocs, bitanLocs, tcLocs, jiLocs, jwLocs;
	vpLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_riverShader.GetShaderProgram()]	 = glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexPosition");

	vnLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_riverShader.GetShaderProgram()]	 = glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexNormal");
	
	tanLocs[m_forwardShader.GetShaderProgram()]	  = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_riverShader.GetShaderProgram()]	 = glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexTangent");

	bitanLocs[m_forwardShader.GetShaderProgram()]	= glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_riverShader.GetShaderProgram()]		= glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexBiTangent");
	
	tcLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_riverShader.GetShaderProgram()]	= glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexTexCoord");
	
	jiLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexJointIndex");
	jiLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexJointIndex");
	jiLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexJointIndex");
	jiLocs[m_riverShader.GetShaderProgram()]	= glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexJointIndex");
	jiLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexJointIndex");
	jiLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexJointIndex");
	
	jwLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexJointWeight");
	jwLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexJointWeight");
	jwLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexJointWeight");
	jwLocs[m_riverShader.GetShaderProgram()]	= glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexJointWeight");
	jwLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexJointWeight");
	jwLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexJointWeight");

	_shaderProg->UseProgram();
	BufferData bufferData[] =
	{
		{ vpLocs,	 3, GL_FLOAT, (const GLvoid*)positionData.data(), (GLsizeiptr)(positionData.size() * sizeof(float)) },
		{ vnLocs,	 3, GL_FLOAT, (const GLvoid*)normalData.data(), (GLsizeiptr)(normalData.size()   * sizeof(float)) },
		{ tanLocs,	 3, GL_FLOAT, (const GLvoid*)tanData.data(), (GLsizeiptr)(tanData.size()   * sizeof(float)) },
		{ bitanLocs, 3, GL_FLOAT, (const GLvoid*)bitanData.data(), (GLsizeiptr)(bitanData.size()   * sizeof(float)) },
		{ tcLocs,	 2, GL_FLOAT, (const GLvoid*)texCoordData.data(), (GLsizeiptr)(texCoordData.size() * sizeof(float)) },
		{ jiLocs,	 4, GL_FLOAT, (const GLvoid*)jointIndexData.data(), (GLsizeiptr)(jointIndexData.size() * sizeof(float)) },
		{ jwLocs,	 4, GL_FLOAT, (const GLvoid*)jointWeightData.data(), (GLsizeiptr)(jointWeightData.size() * sizeof(float)) },
	};

	retbuffer->init(bufferData, sizeof(bufferData) / sizeof(bufferData[0]), _shaderProg->GetShaderProgram());
	retbuffer->setCount((int)positionData.size() / 3);
	
	m_meshs.insert(std::pair<const std::string, Buffer*>(_fileDir, retbuffer));

	return retbuffer;
}

Buffer* GraphicsHigh::AddMesh(ModelToLoadFromSource* _modelToLoad, Shader *_shaderProg)
{
	for (std::map<const std::string, Buffer*>::iterator it = m_meshs.begin(); it != m_meshs.end(); it++)
	{		
		if (it->first == _modelToLoad->key)
			return it->second;
	}

	std::vector<float> positionData = _modelToLoad->positions;
	std::vector<float> normalData = _modelToLoad->normals;
	std::vector<float> tanData = _modelToLoad->tangents;
	std::vector<float> bitanData = _modelToLoad->bitangents;
	std::vector<float> texCoordData = _modelToLoad->texCoords;
	std::vector<float> jointIndexData;
	std::vector<float> jointWeightData;

	Buffer* retbuffer = new Buffer();

	std::map<GLuint, GLuint> vpLocs, vnLocs, tanLocs, bitanLocs, tcLocs, jiLocs, jwLocs;
	vpLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_riverShader.GetShaderProgram()]	 = glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexPosition");

	vnLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_riverShader.GetShaderProgram()]	 = glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexNormal");
	
	tanLocs[m_forwardShader.GetShaderProgram()]	  = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_riverShader.GetShaderProgram()]	 = glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexTangent");

	bitanLocs[m_forwardShader.GetShaderProgram()]	= glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_riverShader.GetShaderProgram()]		= glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexBiTangent");
	
	tcLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_riverShader.GetShaderProgram()]	= glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexTexCoord");
	
	jiLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexJointIndex");
	jiLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexJointIndex");
	jiLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexJointIndex");
	jiLocs[m_riverShader.GetShaderProgram()]	= glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexJointIndex");
	jiLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexJointIndex");
	jiLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexJointIndex");
	
	jwLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexJointWeight");
	jwLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexJointWeight");
	jwLocs[m_interfaceShader.GetShaderProgram()] = glGetAttribLocation(m_interfaceShader.GetShaderProgram(), "VertexJointWeight");
	jwLocs[m_riverShader.GetShaderProgram()]	= glGetAttribLocation(m_riverShader.GetShaderProgram(), "VertexJointWeight");
	jwLocs[m_riverCornerShader.GetShaderProgram()] = glGetAttribLocation(m_riverCornerShader.GetShaderProgram(), "VertexJointWeight");
	jwLocs[m_animationShader.GetShaderProgram()] = glGetAttribLocation(m_animationShader.GetShaderProgram(), "VertexJointWeight");

	_shaderProg->UseProgram();
	BufferData bufferData[] =
	{
		{ vpLocs,	 3, GL_FLOAT, (const GLvoid*)positionData.data(), (GLsizeiptr)(positionData.size() * sizeof(float)) },
		{ vnLocs,	 3, GL_FLOAT, (const GLvoid*)normalData.data(), (GLsizeiptr)(normalData.size()   * sizeof(float)) },
		{ tanLocs,	 3, GL_FLOAT, (const GLvoid*)tanData.data(), (GLsizeiptr)(tanData.size()   * sizeof(float)) },
		{ bitanLocs,     3, GL_FLOAT, (const GLvoid*)bitanData.data(), (GLsizeiptr)(bitanData.size()   * sizeof(float)) },
		{ tcLocs,	 2, GL_FLOAT, (const GLvoid*)texCoordData.data(), (GLsizeiptr)(texCoordData.size() * sizeof(float)) },
		{ jiLocs,	 4, GL_FLOAT, (const GLvoid*)jointIndexData.data(), (GLsizeiptr)(jointIndexData.size() * sizeof(float)) },
		{ jwLocs,	 4, GL_FLOAT, (const GLvoid*)jointWeightData.data(), (GLsizeiptr)(jointWeightData.size() * sizeof(float)) },
	};

	retbuffer->init(bufferData, sizeof(bufferData) / sizeof(bufferData[0]), _shaderProg->GetShaderProgram());
	retbuffer->setCount((int)positionData.size() / 3);
	
	m_meshs.insert(std::pair<const std::string, Buffer*>(_modelToLoad->key, retbuffer));

	return retbuffer;
}

void GraphicsHigh::BufferLightsToGPU()
{
	if (m_directionalLightPtr)
	{
		m_shadowMap->UpdateViewMatrix(vec3(8.0f, 0.0f, 8.0f) - (10.0f*normalize(m_dirLightDirection)), vec3(8.0f, 0.0f, 8.0f));
	}
	BufferLightsToGPU_GD();
}