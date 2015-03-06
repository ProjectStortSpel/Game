#include "GraphicsLow.h"

#include "ModelLoader.h"
#include "ModelExporter.h"
#include <sstream>

using namespace Renderer;
using namespace glm;

GraphicsLow::GraphicsLow()
{
	m_stringToInt = new char[2];
	m_useAnimations = true;
	m_modelIDcounter = 0;
	m_vramUsage = 0;
	m_debugTexFlag = 0;
	m_nrOfLightsToBuffer = 0;
    m_pointerToDirectionalLights = 0;
	m_pointerToPointlights = 0;
}

GraphicsLow::GraphicsLow(Camera _camera, int x, int y) : GraphicDevice(_camera, x, y)
{
	m_stringToInt = new char[2];
	m_useAnimations = true;
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
	delete[](m_stringToInt);
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

    if (!InitGLEW()) { ERRORMSG("GLEW_VERSION_4_0 FAILED.\n INCOMPATIBLE GRAPHICS DRIVER\n"); return false; }
	if (!InitShaders()) { ERRORMSG("INIT SHADERS FAILED\n"); return false; }
	InitRenderLists();
	if (!InitBuffers()) { ERRORMSG("INIT BUFFERS FAILED\n"); return false; }
	if (!InitSkybox()) { ERRORMSG("INIT SKYBOX FAILED\n"); return false; }
	
	CreateShadowMap();
	if (!InitLightBuffers()) { ERRORMSG("INIT LIGHTBUFFER FAILED\n"); return false; }

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

	for (int i = 0; i < m_modelsAnimated.size(); i++)
	{
		if (m_modelsAnimated[i].active) // IS MODEL ACTIVE?
		{
			m_modelsAnimated[i].Update(_dt);
		}
	}
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
		if (m_modelsForward[i].castShadow)
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
	}

	//--------ANIMATED DEFERRED RENDERING !!! ATTENTION: WORK IN PROGRESS !!!
	m_animationShader.UseProgram();
	for (int i = 0; i < m_modelsAnimated.size(); i++)
	{
		for (int j = 0; j < m_modelsAnimated[i].anim.size(); j++)
		{
			std::stringstream ss;
			ss << "anim[" << j << "]";
			m_animationShader.SetUniVariable(ss.str().c_str(), mat4x4, &m_modelsAnimated[i].anim[j]);
			ss.str(std::string());
		}
		m_modelsAnimated[i].Draw((*m_shadowMap->GetViewMatrix()), shadowProjection, &m_animationShader);
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
	glEnable(GL_BLEND);


	//------FORWARD RENDERING--------------------------------------------
	m_forwardShader.UseProgram();
	m_forwardShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
	m_forwardShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);

	m_forwardShader.SetUniVariable("ShadowViewProj", mat4x4, &shadowVP);

	for (int i = 0; i < m_modelsForward.size(); i++)
		m_modelsForward[i].Draw(viewMatrix, mat4(1));

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
		m_modelsWater[i].Draw(viewMatrix, mat4(1));

	//-------Render water corners-------------
	m_riverCornerShader.UseProgram();
	m_riverCornerShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
	m_riverCornerShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);
	m_riverCornerShader.SetUniVariable("ShadowViewProj", mat4x4, &shadowVP);
	m_riverCornerShader.SetUniVariable("ElapsedTime", glfloat, &m_elapsedTime);
	//----DRAW MODELS
	for (int i = 0; i < m_modelsWaterCorners.size(); i++)
		m_modelsWaterCorners[i].Draw(viewMatrix, mat4(1));
	
	
	//--------PARTICLES---------
	glEnable(GL_POINT_SPRITE);
	glDepthMask(GL_FALSE);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	glActiveTexture(GL_TEXTURE1);

	for (std::map<int, ParticleSystem*>::iterator it = m_particleSystems.begin(); it != m_particleSystems.end(); ++it)
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
		m_modelsViewspace[i].Draw(mat4(1), mat4(1));

	// RENDER INTERFACE STUFF
	//glDisable(GL_DEPTH_TEST);
	m_interfaceShader.UseProgram();
	m_interfaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	SortModelsBasedOnDepth(&m_modelsInterface);
	for (int i = 0; i < m_modelsInterface.size(); i++)
		m_modelsInterface[i].Draw(mat4(1), mat4(1));

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
	InitStandardShaders();
	// Animation Deferred pass 1
	m_animationShader.InitShaderProgram();
	m_animationShader.AddShader("content/shaders/lowVSAnimationShader.glsl", GL_VERTEX_SHADER);
	m_animationShader.AddShader("content/shaders/lowFSAnimationShader.glsl", GL_FRAGMENT_SHADER);
	m_animationShader.FinalizeShaderProgram();

	// Forward shader
	m_forwardShader.InitShaderProgram();
	m_forwardShader.AddShader("content/shaders/lowVSForwardShader.glsl", GL_VERTEX_SHADER);
	m_forwardShader.AddShader("content/shaders/lowFSForwardShader.glsl", GL_FRAGMENT_SHADER);
	m_forwardShader.FinalizeShaderProgram();

	// River water shader
	m_riverShader.InitShaderProgram();
	m_riverShader.AddShader("content/shaders/riverShaderVS.glsl", GL_VERTEX_SHADER);
	m_riverShader.AddShader("content/shaders/lowFSForwardShader.glsl", GL_FRAGMENT_SHADER);
	m_riverShader.FinalizeShaderProgram();

	// River water corner shader
	m_riverCornerShader.InitShaderProgram();
	m_riverCornerShader.AddShader("content/shaders/lowVSForwardShader.glsl", GL_VERTEX_SHADER);
	m_riverCornerShader.AddShader("content/shaders/lowRiverCornerFS.glsl", GL_FRAGMENT_SHADER);
	m_riverCornerShader.FinalizeShaderProgram();

	return true;
}

void GraphicsLow::InitRenderLists()
{
	m_renderLists.push_back(RenderList(RENDER_FORWARD, &m_modelsForward, &m_forwardShader));
	m_renderLists.push_back(RenderList(RENDER_VIEWSPACE, &m_modelsViewspace, &m_viewspaceShader));
	m_renderLists.push_back(RenderList(RENDER_INTERFACE, &m_modelsInterface, &m_interfaceShader));
	m_renderLists.push_back(RenderList(RENDER_RIVERWATER, &m_modelsWater, &m_riverShader));
	m_renderLists.push_back(RenderList(RENDER_RIVERWATER_CORNER, &m_modelsWaterCorners, &m_riverCornerShader));
	m_renderLists.push_back(RenderList(RENDER_DEFERRED, &m_modelsForward, &m_forwardShader)); // TODO: Not really a good solution but works
}

bool GraphicsLow::InitBuffers()
{
	InitStandardBuffers();

	return true;
}

bool GraphicsLow::InitLightBuffers()
{
	for (int i = 0; i < 10; i++)
		m_lightDefaults[i] = 0.0;		//init 0.0

	float **tmpPtr = new float*[1];
	BufferPointlights(0, tmpPtr);
	delete [] tmpPtr;

	BufferDirectionalLight(&m_lightDefaults[0]);
	
	return true;
}

void GraphicsLow::BufferPointlights(int _nrOfLights, float **_lightPointers)
{
	if (m_pointerToPointlights)
	{
		delete [] m_pointerToPointlights;
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

		m_riverShader.SetUniVariable("dirlightDirection", vector3, &m_dirLightDirection);
		m_riverShader.SetUniVariable("dirlightIntensity", vector3, &intens);
		m_riverShader.SetUniVariable("dirlightColor", vector3, &color);

		m_riverCornerShader.SetUniVariable("dirlightDirection", vector3, &m_dirLightDirection);
		m_riverCornerShader.SetUniVariable("dirlightIntensity", vector3, &intens);
		m_riverCornerShader.SetUniVariable("dirlightColor", vector3, &color);

		m_animationShader.SetUniVariable("dirlightDirection", vector3, &m_dirLightDirection);
		m_animationShader.SetUniVariable("dirlightIntensity", vector3, &intens);
		m_animationShader.SetUniVariable("dirlightColor", vector3, &color);

		m_shadowMap->UpdateViewMatrix(vec3(8.0f, 0.0f, 8.0f) - (10.0f*normalize(m_dirLightDirection)), vec3(8.0f, 0.0f, 8.0f));
	}
	else
	{
		vec3 zero = vec3(0.0f);
		m_forwardShader.SetUniVariable("dirlightIntensity", vector3, &zero);
		m_forwardShader.SetUniVariable("dirlightColor", vector3, &zero);
		m_riverShader.SetUniVariable("dirlightIntensity", vector3, &zero);
		m_riverShader.SetUniVariable("dirlightColor", vector3, &zero);
		m_riverCornerShader.SetUniVariable("dirlightIntensity", vector3, &zero);
		m_riverCornerShader.SetUniVariable("dirlightColor", vector3, &zero);
		m_animationShader.SetUniVariable("dirlightIntensity", vector3, &zero);
		m_animationShader.SetUniVariable("dirlightColor", vector3, &zero);
	}

	// ------------Pointlights------------
	if (m_pointerToPointlights && m_numberOfPointlights == 1 && m_nrOfLightsToBuffer == 3)
	{
		for (int i = 0; i < 3; i++)
		{

			std::string s;
			sprintf( m_stringToInt, "%d", i );
			s = *m_stringToInt;
			s = "pointlights[" + s + "].Position";
			m_forwardShader.SetUniVariable( s.c_str( ), vector3, &m_lightDefaults[0] );
			m_riverShader.SetUniVariable( s.c_str( ), vector3, &m_lightDefaults[0] );
			m_riverCornerShader.SetUniVariable( s.c_str( ), vector3, &m_lightDefaults[0] );
			m_animationShader.SetUniVariable( s.c_str( ), vector3, &m_lightDefaults[0] );

			sprintf( m_stringToInt, "%d", i );
			s = *m_stringToInt;
			s = "pointlights[" + s + "].Intensity";
			m_forwardShader.SetUniVariable( s.c_str( ), vector3, &m_lightDefaults[3] );
			m_riverShader.SetUniVariable( s.c_str( ), vector3, &m_lightDefaults[3] );
			m_riverCornerShader.SetUniVariable( s.c_str( ), vector3, &m_lightDefaults[3] );
			m_animationShader.SetUniVariable( s.c_str( ), vector3, &m_lightDefaults[3] );

			sprintf( m_stringToInt, "%d", i );
			s = *m_stringToInt;
			s = "pointlights[" + s + "].Color";
			m_forwardShader.SetUniVariable(s.c_str(), vector3, &m_lightDefaults[6]);		
			m_riverShader.SetUniVariable(s.c_str(), vector3, &m_lightDefaults[6]);		
			m_riverCornerShader.SetUniVariable(s.c_str(), vector3, &m_lightDefaults[6]);
			m_animationShader.SetUniVariable( s.c_str( ), vector3, &m_lightDefaults[6] );

			sprintf( m_stringToInt, "%d", i );
			s = *m_stringToInt;
			s = "pointlights[" + s + "].Range";
			m_forwardShader.SetUniVariable( s.c_str( ), glfloat, &m_lightDefaults[9] );
			m_riverShader.SetUniVariable( s.c_str( ), glfloat, &m_lightDefaults[9] );
			m_riverCornerShader.SetUniVariable( s.c_str( ), glfloat, &m_lightDefaults[9] );
			m_animationShader.SetUniVariable( s.c_str( ), glfloat, &m_lightDefaults[9] );

		}

		delete[] m_pointerToPointlights;
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

		for ( int i = 0; i < nrOfLights; i++ )
		{
			std::string s;
			sprintf( m_stringToInt, "%d", i );
			s = *m_stringToInt;
			s = "pointlights[" + s + "].Position";
			m_forwardShader.SetUniVariable( s.c_str( ), vector3, &m_pointerToPointlights[i][0] );
			m_riverShader.SetUniVariable( s.c_str( ), vector3, &m_pointerToPointlights[i][0] );
			m_riverCornerShader.SetUniVariable( s.c_str( ), vector3, &m_pointerToPointlights[i][0] );
			m_animationShader.SetUniVariable( s.c_str( ), vector3, &m_pointerToPointlights[i][0] );


			sprintf( m_stringToInt, "%d", i );
			s = *m_stringToInt;
			s = "pointlights[" + s + "].Intensity";
			m_forwardShader.SetUniVariable( s.c_str( ), vector3, &m_pointerToPointlights[i][3] );
			m_riverShader.SetUniVariable( s.c_str( ), vector3, &m_pointerToPointlights[i][3] );
			m_riverCornerShader.SetUniVariable( s.c_str( ), vector3, &m_pointerToPointlights[i][3] );
			m_animationShader.SetUniVariable( s.c_str( ), vector3, &m_pointerToPointlights[i][3] );

			sprintf( m_stringToInt, "%d", i );
			s = *m_stringToInt;
			s = "pointlights[" + s + "].Color";
			m_forwardShader.SetUniVariable(s.c_str(), vector3, &m_pointerToPointlights[i][6]);		
			m_riverShader.SetUniVariable(s.c_str(), vector3, &m_pointerToPointlights[i][6]);		
			m_riverCornerShader.SetUniVariable(s.c_str(), vector3, &m_pointerToPointlights[i][6]);
			m_animationShader.SetUniVariable(s.c_str(), vector3, &m_pointerToPointlights[i][6]);

			sprintf( m_stringToInt, "%d", i );
			s = *m_stringToInt;
			s = "pointlights[" + s + "].Range";
			m_forwardShader.SetUniVariable(s.c_str(), glfloat, &m_pointerToPointlights[i][9]);	
			m_riverShader.SetUniVariable(s.c_str(), glfloat, &m_pointerToPointlights[i][9]);		
			m_riverCornerShader.SetUniVariable(s.c_str(), glfloat, &m_pointerToPointlights[i][9]);
			m_animationShader.SetUniVariable(s.c_str(), glfloat, &m_pointerToPointlights[i][9]);

		}
		

		delete[] m_pointerToPointlights;

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

	m_riverShader.UseProgram();
	m_riverShader.SetUniVariable("BiasMatrix", mat4x4, m_shadowMap->GetBiasMatrix());
	m_riverShader.CheckUniformLocation("ShadowDepthTex", 10);

	m_riverCornerShader.UseProgram();
	m_riverCornerShader.SetUniVariable("BiasMatrix", mat4x4, m_shadowMap->GetBiasMatrix());
	m_riverCornerShader.CheckUniformLocation("ShadowDepthTex", 10);

	m_animationShader.UseProgram();
	m_animationShader.SetUniVariable("BiasMatrix", mat4x4, m_shadowMap->GetBiasMatrix());
	m_animationShader.CheckUniformLocation("ShadowDepthTex", 10);

	m_vramUsage += (resolution*resolution*sizeof(float));
}

void GraphicsLow::Clear()
{
	m_modelIDcounter = 0;
	
	m_modelsAnimated.clear();
	m_modelsForward.clear();
	m_modelsViewspace.clear();
	m_modelsInterface.clear();
	m_modelsWater.clear();
	m_modelsWaterCorners.clear();

	BufferPointlights(0, 0);
	BufferDirectionalLight(0);

	if (m_pointerToPointlights)
		delete [] m_pointerToPointlights;
	
	m_pointerToPointlights = NULL;
	m_pointerToDirectionalLights = NULL;
	m_numberOfPointlights = 0;
	m_numberOfDirectionalLights = 0;
	
	for (std::map<int, ParticleSystem*>::iterator it = m_particleSystems.begin(); it != m_particleSystems.end(); ++it)
		delete(it->second);
	m_particleSystems.clear();
}