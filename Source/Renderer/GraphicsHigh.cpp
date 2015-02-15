#include "GraphicsHigh.h"

#include "ModelLoader.h"
#include "ModelExporter.h"

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
		std::vector<float> colors(m_modelsDeferred[i].instances.size()*4);

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

		m_modelsDeferred[i].bufferPtr->drawInstanced(0, nrOfInstances, &MVPVector, &normalMatVector, &colors);
	}

	//------Forward------------------------------------
	m_shadowShaderForward.UseProgram();
	//Forward models
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		std::vector<mat4> MVPVector(m_modelsForward[i].instances.size());
		std::vector<mat3> normalMatVector(m_modelsForward[i].instances.size());
		std::vector<float> colors(m_modelsForward[i].instances.size() * 4);

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

		m_modelsForward[i].bufferPtr->drawInstanced(0, nrOfInstances, &MVPVector, &normalMatVector, &colors);
	}
	//------------------------------------------------

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
	glDisable(GL_POLYGON_OFFSET_FILL);
	//------------------------------
}

void GraphicsHigh::Render()
{
		//GLTimer glTimer;
		//glTimer.Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	WriteShadowMapDepth();

//------Render deferred--------------------------------------------------------------------------
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_deferredFBO);
	glViewport(0, 0, m_clientWidth, m_clientHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	m_deferredShader1.UseProgram();
	
	//--------Uniforms-------------------------------------------------------------------------
	mat4 projectionMatrix = *m_camera->GetProjMatrix();

	mat4 viewMatrix = *m_camera->GetViewMatrix();

	m_deferredShader1.SetUniVariable("TexFlag", glint, &m_debugTexFlag);

	//------Render scene (for deferred)-----------------------------------------------------------
	//-- DRAW MODELS
	for (int i = 0; i < m_modelsDeferred.size(); i++)
	{
		std::vector<mat4> MVPVector(m_modelsDeferred[i].instances.size());
		std::vector<mat3> normalMatVector(m_modelsDeferred[i].instances.size());
		std::vector<float> colors(m_modelsDeferred[i].instances.size()*4);

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

				mat4 modelViewMatrix;
				modelViewMatrix = viewMatrix * modelMatrix;

				mat4 mvp = projectionMatrix * modelViewMatrix;
				MVPVector[nrOfInstances] = mvp;
				
				mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
				normalMatVector[nrOfInstances] = normalMatrix;

				//m_deferredShader1.SetUniVariable("BlendColor", vector3, m_modelsDeferred[i].instances[j].color);

				colors[4*nrOfInstances+0] = m_modelsDeferred[i].instances[j].color[0];
				colors[4*nrOfInstances+1] = m_modelsDeferred[i].instances[j].color[1];
				colors[4*nrOfInstances+2] = m_modelsDeferred[i].instances[j].color[2];
				colors[4*nrOfInstances+3] = 0.f;

				nrOfInstances++;
			}
		}
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_modelsDeferred[i].texID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_modelsDeferred[i].norID);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_modelsDeferred[i].speID);

		//m_modelsDeferred[i].bufferPtr->draw();
		m_modelsDeferred[i].bufferPtr->drawInstanced(0, nrOfInstances, &MVPVector, &normalMatVector, &colors);
		glBindTexture(GL_TEXTURE_2D, 0);
	}



	// ---- ANIMATED DEFERED
	m_animationShader.UseProgram();
	
	m_animationShader.SetUniVariable("TexFlag", glint, &m_debugTexFlag);
	
	//------Render scene (for deferred)
	//-- DRAW MODELS
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
			//m_animationShader.SetUniVariable("BlendColor", vector3, m_modelsAnimated[i].color);
	
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

	//m_glTimerValues.push_back(GLTimerValue("Deferred stage1: ", glTimer.Stop()));
	//glTimer.Start();	

	
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

		//m_glTimerValues.push_back(GLTimerValue("Deferred stage2: ", glTimer.Stop()));
		//glTimer.Start();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_forwardFBO);

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

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_dirLightBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_pointlightBuffer);

	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		std::vector<mat4> modelViewVector(m_modelsForward[i].instances.size());
		std::vector<mat3> normalMatVector(m_modelsForward[i].instances.size());
		std::vector<float> colors(m_modelsForward[i].instances.size() * 4);

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

				//m_forwardShader.SetUniVariable("BlendColor", vector3, m_modelsForward[i].instances[j].color);

				colors[4 * nrOfInstances + 0] = m_modelsForward[i].instances[j].color[0];
				colors[4 * nrOfInstances + 1] = m_modelsForward[i].instances[j].color[1];
				colors[4 * nrOfInstances + 2] = m_modelsForward[i].instances[j].color[2];
				colors[4 * nrOfInstances + 3] = 0.f;

				nrOfInstances++;
			}
		}

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].texID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].norID);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].speID);

		m_modelsForward[i].bufferPtr->drawInstanced(0, m_modelsForward[i].instances.size(), &modelViewVector, &normalMatVector, &colors);
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

	for (int i = 0; i < m_particleSystems.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_particleSystems[i]->GetTexHandle());

		mat4 Model = glm::translate(m_particleSystems[i]->GetWorldPos());
		mat4 ModelView = viewMatrix * Model;

		m_particleShader.SetUniVariable("ModelView", mat4x4, &ModelView);
		//GLuint location = glGetUniformLocation(m_particleSystems, "ModelView");	//gets the UniformLocation
		//if (location >= 0){ glUniformMatrix4fv(location, 1, GL_FALSE, &ModelView[0][0]); }

		m_particleSystems[i]->Render(m_dt);
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
		std::vector<float> colors(m_modelsViewspace[i].instances.size() * 4);

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

				colors[4 * nrOfInstances + 0] = m_modelsViewspace[i].instances[j].color[0];
				colors[4 * nrOfInstances + 1] = m_modelsViewspace[i].instances[j].color[1];
				colors[4 * nrOfInstances + 2] = m_modelsViewspace[i].instances[j].color[2];
				colors[4 * nrOfInstances + 3] = 0.f;

				nrOfInstances++;
			}
		}

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].texID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].norID);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].speID);

		m_modelsViewspace[i].bufferPtr->drawInstanced(0, m_modelsViewspace[i].instances.size(), &modelViewVector, &normalMatVector, &colors);
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
		std::vector<float> colors(m_modelsInterface[i].instances.size() * 4);

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

				colors[4 * nrOfInstances + 0] = m_modelsInterface[i].instances[j].color[0];
				colors[4 * nrOfInstances + 1] = m_modelsInterface[i].instances[j].color[1];
				colors[4 * nrOfInstances + 2] = m_modelsInterface[i].instances[j].color[2];
				colors[4 * nrOfInstances + 3] = 0.f;

				nrOfInstances++;
			}
		}

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_modelsInterface[i].texID);

		m_modelsInterface[i].bufferPtr->drawInstanced(0, m_modelsInterface[i].instances.size(), &modelViewVector, &normalMatVector, &colors);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//---------------------------------------------------------------------
	
	if (m_renderSimpleText)
		m_textRenderer.RenderText(m_dt);

	//	//m_glTimerValues.push_back(GLTimerValue("Text Render: ", glTimer.Stop()));
	//	//glTimer.Start();

	// FULL SCREEN QUAD
	m_fullScreenShader.UseProgram();
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, m_shadowMap->GetDepthTexHandle());
	glDrawArrays(GL_POINTS, 0, 1);

	glUseProgram(0);
		//m_glTimerValues.push_back(GLTimerValue("RENDER: ", glTimer.Stop()));

	// Swap in the new buffer
	SDL_GL_SwapWindow(m_window);
}

bool GraphicsHigh::InitSDLWindow()
{
	// WINDOW SETTINGS
	unsigned int	Flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

	int				SizeX = 256 * 5;	//1280
	int				SizeY = 144 * 5;	//720

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
	m_vramUsage += (m_clientWidth*m_clientHeight*sizeof(float)* 4);
	m_vramUsage += (m_clientWidth*m_clientHeight* 1 * 4);

	GLenum drawBuffersDeferred[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffersDeferred);
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

bool GraphicsHigh::InitRandomVector()
{
	int texSizeX, texSizeY;
	m_randomVectors = GraphicDevice::AddTexture("content/textures/vectormap.png", GL_TEXTURE21);

	return true;
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

void GraphicsHigh::CreateParticleSystems()
{
	//glEnable(GL_POINT_SPRITE);
	//m_particleSystems.push_back(new ParticleSystem("fire", vec3(1.f, 0.55f, 1.f), 100, 700, 0.6f, AddTexture("content/textures/fire3.png", GL_TEXTURE1), &m_particleShader));
	m_particleSystems.push_back(new ParticleSystem("smoke", vec3(11.0f, 0.0f, 9.0f), 15, 1800, 1.6f, AddTexture("content/textures/smoke1.png", GL_TEXTURE1), &m_particleShader));
}

bool GraphicsHigh::InitTextRenderer()
{
	int texSizeX, texSizeY;
	GLuint m_textImage = GraphicDevice::AddTexture("content/textures/SimpleText.png", GL_TEXTURE20);
	return m_textRenderer.Init(m_textImage, m_clientWidth, m_clientHeight);
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

int GraphicsHigh::LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType, float* _color)
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
void GraphicsHigh::BufferModel(int _modelId, ModelToLoad* _modelToLoad)
{
	ObjectData obj = ModelLoader::importObject(_modelToLoad->Dir, _modelToLoad->File);

	//if (obj.animated)
	//{
	//	BufferAModel(_modelId, _modelToLoad);
	//	return;
	//}

	Shader *shaderPtr = NULL;
	std::vector<Model> *modelList = NULL;
	if (_modelToLoad->RenderType == RENDER_DEFERRED)
	{
		shaderPtr = &m_deferredShader1;
		modelList = &m_modelsDeferred;
		m_deferredShader1.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_FORWARD)
	{
		shaderPtr = &m_forwardShader;
		modelList = &m_modelsForward;
		m_forwardShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_VIEWSPACE)
	{
		shaderPtr = &m_viewspaceShader;
		modelList = &m_modelsViewspace;
		m_viewspaceShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_INTERFACE)
	{
		shaderPtr = &m_interfaceShader;
		modelList = &m_modelsInterface;
		m_interfaceShader.UseProgram();
	}
	else
	{
		ERRORMSG("ERROR: INVALID RENDER SETTING");
		return;
	}

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh, shaderPtr, false);

	// Import Texture
	GLuint texture = AddTexture(obj.text, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	// Import Normal map
	GLuint normal = AddTexture(obj.norm, GL_TEXTURE2);
	shaderPtr->CheckUniformLocation("normalTex", 2);

	// Import Specc Glow map
	GLuint specular = AddTexture(obj.spec, GL_TEXTURE3);
	shaderPtr->CheckUniformLocation("specularTex", 3);

	Model model = Model(mesh, texture, normal, specular);

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
void GraphicsHigh::BufferAModel(int _modelId, ModelToLoad* _modelToLoad)
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
		model.joints.push_back(Joint(
		joints[i].x0, joints[i].y0, joints[i].z0, joints[i].w0,
		joints[i].x1, joints[i].y1, joints[i].z1, joints[i].w1,
		joints[i].x2, joints[i].y2, joints[i].z2, joints[i].w2,
		joints[i].x3, joints[i].y3, joints[i].z3, joints[i].parent)
		);//joints[i].transform));

	glGenBuffers(1, &model.jointBuffer);

	//for the matrices (modelView + normal)
	m_vramUsage += (16 + 9) * sizeof(float);

	// Push back the model
	modelList->push_back(model);
}
void GraphicsHigh::BufferModels()
{
	for (auto pair : m_modelsToLoad)
	{
		BufferModel(pair.first, pair.second);
		delete(pair.second);
	}
	m_modelsToLoad.clear();
}

bool GraphicsHigh::RemoveModel(int _id)
{
	for (int i = 0; i < m_modelsDeferred.size(); i++)
	{
		for (int j = 0; j < m_modelsDeferred[i].instances.size(); j++)
		{
			if (m_modelsDeferred[i].instances[j].id == _id)
			{
				m_modelsDeferred[i].instances.erase(m_modelsDeferred[i].instances.begin() + j);
				if (m_modelsDeferred[i].instances.size() == 0)
					m_modelsDeferred.erase(m_modelsDeferred.begin() + i);

				return true;
			}
		}
	}
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
bool GraphicsHigh::ActiveModel(int _id, bool _active)
{
	for (int i = 0; i < m_modelsDeferred.size(); i++)
	{
		for (int j = 0; j < m_modelsDeferred[i].instances.size(); j++)
		{
			if (m_modelsDeferred[i].instances[j].id == _id)
			{
				m_modelsDeferred[i].instances[j].active = _active;
				return true;
			}
		}
	}

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
		int hej = 0;
	retbuffer->init(bufferData, bufferDatas);
	retbuffer->setCount((int)positionData.size() / 3);
	
	m_meshs.insert(std::pair<const std::string, Buffer*>(_fileDir, retbuffer));

	return retbuffer;
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

bool GraphicsHigh::BufferModelTexture(int _id, std::string _fileDir, int _textureType)
{
	// TODO: Do this for Interface and Viewspace
	// Model Instance
	Instance instance;
	// Temp Model
	Model model;

	bool found = false;
	int renderType;

	// Find model Instance
	for (int i = 0; i < m_modelsDeferred.size(); i++)
	{
		for (int j = 0; j < m_modelsDeferred[i].instances.size(); j++)
		{
			if (m_modelsDeferred[i].instances[j].id == _id)
			{
				instance = m_modelsDeferred[i].instances[j];
				model = Model(
					m_modelsDeferred[i].bufferPtr,
					m_modelsDeferred[i].texID,
					m_modelsDeferred[i].norID,
					m_modelsDeferred[i].speID
					);
				found = true;
				renderType = RENDER_DEFERRED;
				m_modelsDeferred[i].instances.erase(m_modelsDeferred[i].instances.begin() + j);
				if (m_modelsDeferred[i].instances.size() == 0)
					m_modelsDeferred.erase(m_modelsDeferred.begin() + i);
			}
			if (found) break;
		}
		if (found) break;
	}
	if (!found)
	{
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
		m_deferredShader1.CheckUniformLocation("diffuseTex", 1);
	}
	else if (_textureType == TEXTURE_NORMAL)
	{
		model.norID = GraphicDevice::AddTexture(_fileDir, GL_TEXTURE2);
		m_deferredShader1.CheckUniformLocation("normalTex", 2);
	}
	else if (_textureType == TEXTURE_SPECULAR)
	{
		model.speID = GraphicDevice::AddTexture(_fileDir, GL_TEXTURE3);
		m_deferredShader1.CheckUniformLocation("specularTex", 3);
	}

	// Check if our new Model type already exists and add instance
	if (renderType == RENDER_DEFERRED)
	{
		for (int i = 0; i < m_modelsDeferred.size(); i++)
		{
			if (m_modelsDeferred[i] == model)
			{
				m_modelsDeferred[i].instances.push_back(instance);
				return true;
			}
		}
	}
	else if (renderType == RENDER_FORWARD)
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
	if (renderType == RENDER_DEFERRED)
		m_modelsDeferred.push_back(model);
	else if (renderType == RENDER_FORWARD)
		m_modelsForward.push_back(model);
	else if (renderType == RENDER_VIEWSPACE)
		m_modelsViewspace.push_back(model);
	else if (renderType == RENDER_INTERFACE)
		m_modelsInterface.push_back(model);

	return true;
}

void GraphicsHigh::UpdateTextureIndex(GLuint newTexture, GLuint oldTexture)
{
	for (Model& m : m_modelsDeferred)
		if (m.texID == oldTexture)
			m.texID = newTexture;
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
