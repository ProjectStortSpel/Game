#include "GraphicsHigh.h"

using namespace Renderer;
using namespace glm;

GraphicsHigh::GraphicsHigh()
{
	m_renderSimpleText = true;
	m_modelIDcounter = 0;
	m_vramUsage = 0;
	m_debugTexFlag = 0;
	m_nrOfLights = 0;
	m_pointerToDirectionalLights = 0;
	m_pointerToPointlights = 0;
}

GraphicsHigh::GraphicsHigh(Camera _camera, int x, int y) : GraphicDevice(_camera, x, y)
{
	m_renderSimpleText = true;
	m_modelIDcounter = 0;
	m_vramUsage = 0;
	m_debugTexFlag = 0;
	m_nrOfLights = 0;
	m_pointerToDirectionalLights = 0;
	m_pointerToPointlights = 0;
}

GraphicsHigh::~GraphicsHigh()
{
	delete(m_camera);
	delete(m_shadowMap);
	// Delete buffers
	for (std::map<const std::string, Buffer*>::iterator it = m_meshs.begin(); it != m_meshs.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}

	glDeleteBuffers(1, &m_pointlightBuffer);
	glDeleteBuffers(1, &m_dirLightBuffer);

}

bool GraphicsHigh::Init()
{
	if (!InitSDLWindow()) { ERRORMSG("INIT SDL WINDOW FAILED\n"); return false; }

	if (!m_SDLinitialized)
		m_camera = new Camera(m_clientWidth, m_clientHeight);

	if (!InitGLEW()) { SDL_Log("GLEW_VERSION_4_3 FAILED"); return false; }
	if (!InitShaders()) { ERRORMSG("INIT SHADERS FAILED\n"); return false; }
	InitRenderLists();
	if (!InitDeferred()) { ERRORMSG("INIT DEFERRED FAILED\n"); return false; }	
	if (!InitBuffers()) { ERRORMSG("INIT BUFFERS FAILED\n"); return false; }
	if (!InitForward()) { ERRORMSG("INIT FORWARD FAILED\n"); return false; }
	if (!InitSkybox()) { ERRORMSG("INIT SKYBOX FAILED\n"); return false; }
	if (!InitRandomVector()) { ERRORMSG("INIT RANDOMVECTOR FAIELD\n"); return false; }
	if (!InitTextRenderer()) { ERRORMSG("INIT TEXTRENDERER FAILED\n"); return false; }
		m_vramUsage += (m_textRenderer.GetArraySize() * sizeof(int));
	
	CreateShadowMap();
	if (!InitLightBuffers()) { ERRORMSG("INIT LIGHTBUFFER FAILED\n"); return false; }

	CreateParticleSystems();
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	TextRenderer::Init();
	
    
	return true;
}
#pragma region Inits
bool GraphicsHigh::InitGLEW()
{
	m_glContext = SDL_GL_CreateContext(m_window);

	if (glewInit() != 0) return false;

#ifdef WIN32
	if (!GLEW_VERSION_4_3) { return false; }
#else
	if (!glewIsSupported("GL_VERSION_4_3")) { return false; }
#endif

	SDL_GL_SetSwapInterval(1);

	return true;
}
bool GraphicsHigh::InitShaders()
{
	// Animation Deferred pass 1
	m_animationShader.InitShaderProgram();
	m_animationShader.AddShader("content/shaders/VSAnimationShader.glsl", GL_VERTEX_SHADER);
	m_animationShader.AddShader("content/shaders/FSAnimationShader.glsl", GL_FRAGMENT_SHADER);
	m_animationShader.FinalizeShaderProgram();

	// Deferred pass 1
	m_deferredShader1.InitShaderProgram();
	m_deferredShader1.AddShader("content/shaders/VSDeferredPass1.glsl", GL_VERTEX_SHADER);
	m_deferredShader1.AddShader("content/shaders/FSDeferredPass1.glsl", GL_FRAGMENT_SHADER);
	m_deferredShader1.FinalizeShaderProgram();

	// Deferred pass 2 ( compute shader )
	m_compDeferredPass2Shader.InitShaderProgram();
	m_compDeferredPass2Shader.AddShader("content/shaders/highCSDeferredPass2.glsl", GL_COMPUTE_SHADER);
	m_compDeferredPass2Shader.FinalizeShaderProgram();

	// SkyBox
	m_skyBoxShader.InitShaderProgram();
	m_skyBoxShader.AddShader("content/shaders/skyboxShaderVS.glsl", GL_VERTEX_SHADER);
	m_skyBoxShader.AddShader("content/shaders/skyboxShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_skyBoxShader.FinalizeShaderProgram();

	// Full Screen Quad
	m_fullScreenShader.InitShaderProgram();
	m_fullScreenShader.AddShader("content/shaders/fullscreen.vs", GL_VERTEX_SHADER);
	m_fullScreenShader.AddShader("content/shaders/fullscreen.gs", GL_GEOMETRY_SHADER);
	m_fullScreenShader.AddShader("content/shaders/fullscreen.ps", GL_FRAGMENT_SHADER);
	m_fullScreenShader.FinalizeShaderProgram();

	// Forward shader
	m_forwardShader.InitShaderProgram();
	m_forwardShader.AddShader("content/shaders/VSForwardShader.glsl", GL_VERTEX_SHADER);
	m_forwardShader.AddShader("content/shaders/FSForwardShader.glsl", GL_FRAGMENT_SHADER);
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

	// ShadowShader deferred geometry
	m_shadowShaderDeferred.InitShaderProgram();
	m_shadowShaderDeferred.AddShader("content/shaders/shadowShaderDeferredVS.glsl", GL_VERTEX_SHADER);
	m_shadowShaderDeferred.AddShader("content/shaders/shadowShaderDeferredFS.glsl", GL_FRAGMENT_SHADER);
	m_shadowShaderDeferred.FinalizeShaderProgram();

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
void GraphicsHigh::InitRenderLists()
{
	m_renderLists.push_back(RenderList(RENDER_DEFERRED, &m_modelsDeferred, &m_deferredShader1));
	m_renderLists.push_back(RenderList(RENDER_FORWARD, &m_modelsForward, &m_forwardShader));
	m_renderLists.push_back(RenderList(RENDER_VIEWSPACE, &m_modelsViewspace, &m_viewspaceShader));
	m_renderLists.push_back(RenderList(RENDER_INTERFACE, &m_modelsInterface, &m_interfaceShader));
}
bool GraphicsHigh::InitDeferred()
{
	m_deferredShader1.UseProgram();

	// Create and bind the FBO
	glGenFramebuffers(1, &m_deferredFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_deferredFBO);

	// The depth buffer
	CreateDepthTex(m_depthBuf);
	// The normal and color buffers
	CreateGBufTex(GL_TEXTURE0, GL_RGBA32F, m_normTex); // Normal
	CreateGBufTex(GL_TEXTURE1, GL_RGBA8, m_colorTex); // Color 

	// Attach the images to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuf, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_normTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_colorTex, 0);

	// Add vramUsage calc if adding a new g-buffer texture
	m_vramUsage += (m_clientWidth*m_clientHeight*sizeof(float));
	m_vramUsage += (m_clientWidth*m_clientHeight*sizeof(float) * 4);
	m_vramUsage += (m_clientWidth*m_clientHeight * 1 * 4);

	GLenum drawBuffersDeferred[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffersDeferred);
	return true;
}
bool GraphicsHigh::InitBuffers()
{
	m_compDeferredPass2Shader.UseProgram();

	//------ Compute shader input images --------
	//normal
	glBindImageTexture(0, m_normTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	//color
	glBindImageTexture(1, m_colorTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	//depth
	m_compDeferredPass2Shader.CheckUniformLocation("DepthTex", 0);
	//-------------------------------------------

	// Output ImageBuffer
	glGenTextures(1, &m_outputImage);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_outputImage);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, m_clientWidth, m_clientHeight);

	glBindImageTexture(5, m_outputImage, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	m_vramUsage += (m_clientWidth * m_clientHeight * sizeof(float) * 4);

	// FULL SCREEN QUAD
	m_fullScreenShader.CheckUniformLocation("output_image", 5);

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
bool GraphicsHigh::InitForward()
{
	//m_forwardShader.UseProgram();

	// Create and bind the FBO
	glGenFramebuffers(1, &m_forwardFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_forwardFBO);

	// Attach the images to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuf, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_outputImage, 0);

	GLenum drawBufferForward = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &drawBufferForward);

	return true;
}
bool GraphicsHigh::InitRandomVector()
{
	int texSizeX, texSizeY;
	m_randomVectors = GraphicDevice::AddTexture("content/textures/vectormap.png", GL_TEXTURE21);

	return true;
}
bool GraphicsHigh::InitTextRenderer()
{
	int texSizeX, texSizeY;
	GLuint m_textImage = GraphicDevice::AddTexture("content/textures/SimpleText.png", GL_TEXTURE20);
	return m_textRenderer.Init(m_textImage, m_clientWidth, m_clientHeight);
}
bool GraphicsHigh::InitLightBuffers()
{
	glGenBuffers(1, &m_pointlightBuffer);

	for (int i = 0; i < 10; i++)
		m_lightDefaults[i] = 0.0;		//init 0.0

	if (m_pointlightBuffer < 0)
		return false;

	float ** tmparray = new float*[1];
	tmparray[0] = &m_lightDefaults[0];

	BufferPointlights(0, tmparray);

	delete(tmparray);

	glGenBuffers(1, &m_dirLightBuffer);
	if (m_dirLightBuffer < 0)
		return false;

	BufferDirectionalLight(&m_lightDefaults[0]);

	return true;
}
#pragma endregion in the order they are initialized

void GraphicsHigh::Update(float _dt)
{
	m_dt = _dt; m_fps = 1 / _dt;

	m_camera->Update(_dt);

	// PRINT m_glTimerValues
	for (int i = 0; i < m_glTimerValues.size(); i++)
	{
		std::stringstream output;
		int x = 0;
		int y = 10;
		output << m_glTimerValues[i].name << m_glTimerValues[i].ms;
		m_textRenderer.RenderSimpleText(output.str(), x, y + i);
	}
	m_glTimerValues.clear();

	BufferModels();
	BufferLightsToGPU();
	BufferSurfaces();
	BufferModelTextures();
	BufferParticleSystems();
}

void GraphicsHigh::WriteShadowMapDepth()
{
	//------- Write shadow maps depths ----------
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMap->GetShadowFBOHandle());
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_shadowMap->GetResolution(), m_shadowMap->GetResolution());

	m_shadowShaderDeferred.UseProgram();

	//glCullFace(GL_FRONT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(4.5, 18000.0);

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, 0);

	mat4 shadowProjection = *m_shadowMap->GetProjectionMatrix();
	//----Render deferred geometry-----------
	for (int i = 0; i < m_modelsDeferred.size(); i++)
	{
		std::vector<mat4> MVPVector(m_modelsDeferred[i].instances.size());
		std::vector<mat3> normalMatVector(m_modelsDeferred[i].instances.size());

		int nrOfInstances = 0;

		for (int j = 0; j < m_modelsDeferred[i].instances.size(); j++)
		{
			if (m_modelsDeferred[i].instances[j].active) // IS MODEL ACTIVE?
			{
				mat4 modelMatrix;
				if (m_modelsDeferred[i].instances[j].modelMatrix == NULL)
					modelMatrix = glm::translate(glm::vec3(1));
				else
					modelMatrix = *m_modelsDeferred[i].instances[j].modelMatrix;

				mat4 mvp = shadowProjection * (*m_shadowMap->GetViewMatrix()) * modelMatrix;
				MVPVector[nrOfInstances] = mvp;

				nrOfInstances++;
			}
		}

		m_modelsDeferred[i].bufferPtr->drawInstanced(0, nrOfInstances, &MVPVector, &normalMatVector, 0);
	}

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

		m_modelsForward[i].bufferPtr->drawInstanced(0, nrOfInstances, &MVPVector, &normalMatVector, 0);
	}
	//------------------------------------------------

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
	glDisable(GL_POLYGON_OFFSET_FILL);
	//------------------------------
}

void GraphicsHigh::Render()
{
	// Get Camera matrices
	mat4 projectionMatrix = *m_camera->GetProjMatrix();
	mat4 viewMatrix = *m_camera->GetViewMatrix();

	// --
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	WriteShadowMapDepth();

	//--------DEFERRED RENDERING
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_deferredFBO);
	glViewport(0, 0, m_clientWidth, m_clientHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//----Uniforms
	m_deferredShader1.UseProgram();
	m_deferredShader1.SetUniVariable("TexFlag", glint, &m_debugTexFlag);
	//----DRAW MODELS
	for (int i = 0; i < m_modelsDeferred.size(); i++)
		m_modelsDeferred[i].Draw(viewMatrix, projectionMatrix);

	//--------ANIMATED DEFERRED RENDERING !!! ATTENTION: WORK IN PROGRESS !!!
	//----Uniforms
	m_animationShader.UseProgram();
	m_animationShader.SetUniVariable("TexFlag", glint, &m_debugTexFlag);
	//----DRAW MODELS
	for (int i = 0; i < m_modelsAnimated.size(); i++)
	{
		if (m_modelsAnimated[i].active) // IS MODEL ACTIVE?
		{
			mat4 modelMatrix;
			if (m_modelsAnimated[i].modelMatrix == NULL)
				modelMatrix = glm::translate(glm::vec3(1));
			else
				modelMatrix = *m_modelsAnimated[i].modelMatrix;
	
			mat4 vp = projectionMatrix * viewMatrix;
			mat4 modelViewMatrix = modelMatrix;
			mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
			m_animationShader.SetUniVariable("BlendColor", vector3, m_modelsAnimated[i].color);
	
			m_animationShader.SetUniVariable("M", mat4x4, &modelViewMatrix);
			m_animationShader.SetUniVariable("VP", mat4x4, &vp);
			m_animationShader.SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);
	
	
	
	
			float *joint_data = new float[m_modelsAnimated[i].joints.size() * 16];
			
			for (int j = 0; j < m_modelsAnimated[i].joints.size(); j++)
			{
				memcpy(&joint_data[16 * j], &m_modelsAnimated[i].joints[j], 16 * sizeof(float));
			}
			
			int joint_data_size = 16 * m_modelsAnimated[i].joints.size() * sizeof(float);
			
			glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, m_modelsAnimated[i].jointBuffer, 0, joint_data_size);
			glBufferData(GL_SHADER_STORAGE_BUFFER, joint_data_size, joint_data, GL_STATIC_DRAW);
			
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_modelsAnimated[i].jointBuffer);
	
			delete joint_data;
	
	

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_modelsAnimated[i].texID);
	
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_modelsAnimated[i].norID);
	
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_modelsAnimated[i].speID);
	
			m_modelsAnimated[i].bufferPtr->draw();

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//--------------------------End of pass1--------------------------------

	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	//----Compute shader (pass 2)------------------------------------------------
	m_compDeferredPass2Shader.UseProgram();

	m_compDeferredPass2Shader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);
	mat4 inverseProjection = glm::inverse(projectionMatrix);
	m_compDeferredPass2Shader.SetUniVariable("InvProjection", mat4x4, &inverseProjection);
	
	mat4 shadowVP = (*m_shadowMap->GetProjectionMatrix()) * (*m_shadowMap->GetViewMatrix());
	m_compDeferredPass2Shader.SetUniVariable("ShadowViewProj", mat4x4, &shadowVP);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_depthBuf);

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap->GetDepthTexHandle());

	glBindImageTexture(1, m_colorTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

	glBindImageTexture(2, m_randomVectors, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

	//---Light buffers----------
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_dirLightBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_pointlightBuffer);
	//--------------------------
	
	glDispatchCompute(m_clientWidth * 0.0625, m_clientHeight * 0.0625, 1); // 1/16 = 0.0625
	//---------------------------------------------------------------------------

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_forwardFBO);

	glEnable(GL_DEPTH_TEST);

	// DRAW SKYBOX
	glDisable(GL_CULL_FACE);
	m_skyBoxShader.UseProgram();
	m_skybox->Draw(m_skyBoxShader.GetShaderProgram(), m_camera);
	glEnable(GL_CULL_FACE);
	// -----------

	//--------FORWARD RENDERING
	glEnable(GL_BLEND);
	//----Uniforms
	m_forwardShader.UseProgram();
	m_forwardShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
	m_forwardShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);
	m_forwardShader.SetUniVariable("ShadowViewProj", mat4x4, &shadowVP);
	//----Lights
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_dirLightBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_pointlightBuffer);
	//----DRAW MODELS
	for (int i = 0; i < m_modelsForward.size(); i++)
		m_modelsForward[i].Draw(viewMatrix, mat4(1));


	//--------PARTICLES---------
	glEnable(GL_POINT_SPRITE);
	glDepthMask(GL_FALSE);

	glBlendColor(0.95, 0.95, 0.95, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_CONSTANT_COLOR);
	
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	m_particleShader.UseProgram();

	m_particleShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

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


	//--------VIEWSPACE RENDERING
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	//----Uniforms
	m_viewspaceShader.UseProgram();
	m_viewspaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
	//----DRAW MODELS
	SortModelsBasedOnDepth(&m_modelsViewspace);
	for (int i = 0; i < m_modelsViewspace.size(); i++)
		m_modelsViewspace[i].Draw(mat4(1), mat4(1));

	//--------INTERFACE RENDERING
	//----Uniforms
	m_interfaceShader.UseProgram();
	m_interfaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
	//----DRAW MODELS
	SortModelsBasedOnDepth(&m_modelsInterface);
	for (int i = 0; i < m_modelsInterface.size(); i++)
		m_modelsInterface[i].Draw(mat4(1), mat4(1));

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//---------------------------------------------------------------------
	
	//--------SIMPLETEXT RENDERING
	if (m_renderSimpleText)
		m_textRenderer.RenderText(m_dt);

	//-------- FULL SCREEN QUAD RENDERING
	m_fullScreenShader.UseProgram();
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, m_shadowMap->GetDepthTexHandle());
	glDrawArrays(GL_POINTS, 0, 1);

	glUseProgram(0);

	vec4 vert1 = vec4(0, 0, 0, 1);
	vec4 vert2 = vec4(0, 15, 0, 1);
	for (int i = 0; i < m_modelsAnimated.size(); i++)
	{
		if (m_modelsAnimated[i].active) // IS MODEL ACTIVE?
		{
			vert1 = projectionMatrix * viewMatrix * vert1;
			vert2 = projectionMatrix * viewMatrix * vert2;

			glLineWidth(2.5);
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(vert1.x, vert1.y, vert1.z);
			glVertex3f(vert2.x, vert2.y, vert2.z);
			glEnd();
		}
	}


	// Swap in the new buffer
	SDL_GL_SwapWindow(m_window);
}

void GraphicsHigh::CreateGBufTex(GLenum texUnit, GLenum internalFormat, GLuint &texid) {
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	//glTexStorage2D(GL_TEXTURE_2D, 1, format, m_clientWidth, m_clientHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_clientWidth, m_clientHeight, 0, GL_RGBA, GL_FLOAT, NULL);
}

void GraphicsHigh::CreateDepthTex(GLuint &texid) {
	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_clientWidth, m_clientHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
}

void GraphicsHigh::BufferPointlights(int _nrOfLights, float **_lightPointers)
{
	if (m_pointerToPointlights)
		delete m_pointerToPointlights;

	if (_nrOfLights == 0)
	{
		m_pointerToPointlights = new float*[1];
		m_pointerToPointlights[0] = &m_lightDefaults[0];
		m_numberOfPointlights = 1;
		return;
	}

	m_pointerToPointlights = _lightPointers;
	m_numberOfPointlights = _nrOfLights;

}

void GraphicsHigh::BufferDirectionalLight(float *_lightPointer)
{
	if (_lightPointer == 0)
		m_pointerToDirectionalLights = &m_lightDefaults[0];
	else
		m_pointerToDirectionalLights = _lightPointer;
}

void GraphicsHigh::BufferLightsToGPU()
{
	if (m_pointerToPointlights)
	{
		m_vramUsage -= m_numberOfPointlights * 10 * sizeof(float);
		m_nrOfLights = m_numberOfPointlights;
		float *pointlight_data = new float[m_numberOfPointlights * 10];

		for (int i = 0; i < m_numberOfPointlights; i++)
		{
			memcpy(&pointlight_data[10 * i], m_pointerToPointlights[i], 10 * sizeof(float));
		}

		int point_light_data_size = 10 * m_numberOfPointlights * sizeof(float);

		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 4, m_pointlightBuffer, 0, point_light_data_size);
		glBufferData(GL_SHADER_STORAGE_BUFFER, point_light_data_size, pointlight_data, GL_STATIC_DRAW);
		m_vramUsage += m_numberOfPointlights * 10 * sizeof(float);

		delete pointlight_data;
		delete m_pointerToPointlights;
		m_pointerToPointlights = 0;
	}


	if (m_pointerToDirectionalLights)
	{
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, m_dirLightBuffer, 0, 9 * sizeof(float));
		glBufferData(GL_SHADER_STORAGE_BUFFER, 9 * sizeof(float), m_pointerToDirectionalLights, GL_STATIC_DRAW);

		m_dirLightDirection = vec3(m_pointerToDirectionalLights[0], m_pointerToDirectionalLights[1], m_pointerToDirectionalLights[2]);
		m_shadowMap->UpdateViewMatrix(vec3(8.0f, 0.0f, 8.0f) - (10.0f*normalize(m_dirLightDirection)), vec3(8.0f, 0.0f, 8.0f));

		m_pointerToDirectionalLights = 0;
	}

}

void GraphicsHigh::CreateShadowMap()
{
	int resolution = 2048*2;
	m_dirLightDirection = vec3(0.0, -1.0, 1.0);
	vec3 midMap = vec3(8.0, 0.0, 8.0);
	vec3 lightPos = midMap - (10.0f*normalize(m_dirLightDirection));
	m_shadowMap = new ShadowMap(lightPos, lightPos + normalize(m_dirLightDirection), resolution);
	m_shadowMap->CreateShadowMapTexture(GL_TEXTURE10);

	m_compDeferredPass2Shader.UseProgram();
	m_compDeferredPass2Shader.SetUniVariable("BiasMatrix", mat4x4, m_shadowMap->GetBiasMatrix());
	m_compDeferredPass2Shader.CheckUniformLocation("ShadowDepthTex", 10);

	m_forwardShader.UseProgram();
	m_forwardShader.SetUniVariable("BiasMatrix", mat4x4, m_shadowMap->GetBiasMatrix());
	m_forwardShader.CheckUniformLocation("ShadowDepthTex", 10);

	m_vramUsage += (resolution*resolution*sizeof(float));
}

bool GraphicsHigh::RenderSimpleText(std::string _text, int _x, int _y)
{
	return m_textRenderer.RenderSimpleText(_text, _x, _y);
}
void GraphicsHigh::SetSimpleTextColor(float _r, float _g, float _b, float _a)
{
	m_textRenderer.SetSimpleTextColor(vec4(_r,_g,_b,_a));
}
void GraphicsHigh::SetDisco()
{
	m_textRenderer.Disco();
}
void GraphicsHigh::ToggleSimpleText(bool _render)
{
	m_renderSimpleText = _render;
}
void GraphicsHigh::ToggleSimpleText()
{
	m_renderSimpleText = !m_renderSimpleText;
}

void GraphicsHigh::Clear()
{
	m_modelIDcounter = 0;
	
	m_modelsDeferred.clear();
	m_modelsForward.clear();
	m_modelsViewspace.clear();
	m_modelsInterface.clear();

	float **tmpPtr = new float*[1];
	BufferPointlights(0, tmpPtr);
	delete tmpPtr;
	BufferDirectionalLight(0);

	//if (m_pointerToPointlights)
		//delete m_pointerToPointlights;
}