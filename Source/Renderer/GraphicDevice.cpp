#include "GraphicDevice.h"

#define STB_IMAGE_IMPLEMENTATION
#include "TextureLoader.h"
#include "ModelLoader.h"

using namespace Renderer;
using namespace glm;

float testArray[30];

GraphicDevice::GraphicDevice()
{
	m_modelIDcounter = 0;
	m_camera = new Camera();
	m_vramUsage = 0;
	m_debugTexFlag = 0;
	m_nrOfLights = 0;
}

GraphicDevice::~GraphicDevice()
{
	delete(m_camera);
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
	if (!InitGLEW()) { ERRORMSG("GLEW_VERSION_4_3 FAILED\n"); return false; }
	if (!InitShaders()) { ERRORMSG("INIT SHADERS FAILED\n"); return false; }
	if (!InitDeferred()) { ERRORMSG("INIT DEFERRED FAILED\n"); return false; }	
	if (!InitBuffers()) { ERRORMSG("INIT BUFFERS FAILED\n"); return false; }
	if (!InitForward()) { ERRORMSG("INIT FORWARD FAILED\n"); return false; }
	if (!InitTextRenderer()) { ERRORMSG("INIT TEXTRENDERER FAILED\n"); return false; }
		m_vramUsage += (m_textRenderer.GetArraySize() * sizeof(int));
	if (!InitLightBuffer()) { ERRORMSG("INIT LIGHTBUFFER FAILED\n"); return false; }
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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
	m_dt = _dt; m_fps = 1 / _dt;

	// Test kod för att rendera text
	std::stringstream sstm;
	sstm << m_fps << " fps";
	m_textRenderer.RenderSimpleText(sstm.str(), 0, 0);

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

	std::stringstream vram;
	vram << "VRAM usage: " << ((float)m_vramUsage/1024.f)/1024.f << " Mb ";
	m_textRenderer.RenderSimpleText(vram.str(), 20, 0);
}

void GraphicDevice::Render()
{
		//GLTimer glTimer;
		//glTimer.Start();
	//------Render deferred--------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_deferredFBO);
	glViewport(0, 0, m_clientWidth, m_clientHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.15, 1.0f);
	
	m_deferredShader1.UseProgram();
	
	//--------Uniforms-------------------------------------------------------------------------
	mat4 projectionMatrix = glm::perspective(45.0f, (float)m_clientWidth / (float)m_clientHeight, 0.2f, 100.f);
	m_deferredShader1.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	mat4 viewMatrix = *m_camera->GetViewMatrix();

	m_deferredShader1.SetUniVariable("TexFlag", glint, &m_debugTexFlag);

	//------Render scene (for deferred)-----------------------------------------------------------
	//-- DRAW MODELS
	for (int i = 0; i < m_modelsDeferred.size(); i++)
	{
		std::vector<mat4> modelViewVector(m_modelsDeferred[i].instances.size());
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

				mat4 modelViewMatrix = viewMatrix * modelMatrix;
				modelViewVector[nrOfInstances] = modelViewMatrix;
				
				mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
				normalMatVector[nrOfInstances] = normalMatrix;

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
		m_modelsDeferred[i].bufferPtr->drawInstanced(0, nrOfInstances, &modelViewVector, &normalMatVector);
		glBindTexture(GL_TEXTURE_2D, 0);
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
	m_compDeferredPass2Shader.SetUniVariable("invProjection", mat4x4, &inverseProjection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_depthBuf);

	glBindImageTexture(1, m_colorTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_pointlightBuffer);
	//glBufferData(GL_SHADER_STORAGE_BUFFER, point_light_data_size, pointlight_data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_pointlightBuffer);
	
	glDispatchCompute(m_clientWidth * 0.0625, m_clientHeight * 0.0625, 1); // 1/16 = 0.0625
	//---------------------------------------------------------------------------

		//m_glTimerValues.push_back(GLTimerValue("Deferred stage2: ", glTimer.Stop()));
		//glTimer.Start();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	//------FORWARD RENDERING--------------------------------------------
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_forwardFBO);
	glViewport(0, 0, m_clientWidth, m_clientHeight);

	m_forwardShader.UseProgram();
	m_forwardShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
	m_forwardShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);

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

				mat4 modelViewMatrix = viewMatrix * modelMatrix;
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

		//m_modelsDeferred[i].bufferPtr->draw();
		m_modelsForward[i].bufferPtr->drawInstanced(0, m_modelsForward[i].instances.size(), &modelViewVector, &normalMatVector);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// DRAW SKYBOX
	//m_skyBoxShader.UseProgram();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_skyBox);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindTexture(GL_TEXTURE_2D, 0);
	// -----------

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//---------------------------------------------------------------------

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	m_textRenderer.RenderText(m_dt);

	//	//m_glTimerValues.push_back(GLTimerValue("Text Render: ", glTimer.Stop()));
	//	//glTimer.Start();

	// FULL SCREEN QUAD
	m_fullScreenShader.UseProgram();
	glDrawArrays(GL_POINTS, 0, 1);

	glUseProgram(0);
		//m_glTimerValues.push_back(GLTimerValue("Full Screen: ", glTimer.Stop()));

	// Swap in the new buffer
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
	// WINDOW SETTINGS
	unsigned int	Flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	const char*		Caption = "SDL Window";
	int				PosX = 650;
	int				PosY = 170;

	int				SizeX = 256 * 5;	//1280
	int				SizeY = 144 * 5;	//720

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
		std::cout << SDL_GetError() << std::endl;
		return false;
	}

	// PLATFORM SPECIFIC CODE
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow(Caption, PosX, PosY, SizeX, SizeY, Flags);

	if (m_window == NULL){
		std::cout << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GetWindowSize(m_window, &m_clientWidth, &m_clientHeight);

	return true;
}

bool GraphicDevice::InitGLEW()
{
	m_glContext = SDL_GL_CreateContext(m_window);

	if (glewInit() != 0) return false;

#ifdef WIN32
	if (!GLEW_VERSION_4_3) { return false; }
#else
	if (!glewIsSupported("GL_VERSION_4_3")) { return false; }
#endif

	SDL_GL_SetSwapInterval(0);
	
	return true;
}

void GraphicDevice::CreateGBufTex(GLenum texUnit, GLenum internalFormat, GLuint &texid) {
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

void GraphicDevice::CreateDepthTex(GLuint &texid) {
	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_clientWidth, m_clientHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
}

bool GraphicDevice::InitDeferred()
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

bool GraphicDevice::InitForward()
{
	m_forwardShader.UseProgram();

	// Create and bind the FBO
	glGenFramebuffers(1, &m_forwardFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_forwardFBO);

	// Attach the images to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuf, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_outputImage, 0);

	GLenum drawBufferForward = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &drawBufferForward);
	
	// skybox
	//m_skyBox = AddTexture("content/textures/skybox.jpg", GL_TEXTURE1);
	//m_skyBoxShader.CheckUniformLocation("diffuseTex", 1);

	return true;
}

bool GraphicDevice::InitShaders()
{
	// Deferred pass 1
	m_deferredShader1.InitShaderProgram();
	m_deferredShader1.AddShader("content/shaders/VSDeferredPass1.glsl", GL_VERTEX_SHADER);
	m_deferredShader1.AddShader("content/shaders/FSDeferredPass1.glsl", GL_FRAGMENT_SHADER);
	m_deferredShader1.FinalizeShaderProgram();

	// Deferred pass 2 ( compute shader )
	m_compDeferredPass2Shader.InitShaderProgram();
	m_compDeferredPass2Shader.AddShader("content/shaders/CSDeferredPass2.glsl", GL_COMPUTE_SHADER);
	m_compDeferredPass2Shader.FinalizeShaderProgram();

	// Sky Box
	m_skyBoxShader.InitShaderProgram();
	m_skyBoxShader.AddShader("content/shaders/skyboxvs.glsl", GL_VERTEX_SHADER);
	m_skyBoxShader.AddShader("content/shaders/skyboxgs.glsl", GL_GEOMETRY_SHADER);
	m_skyBoxShader.AddShader("content/shaders/skyboxps.glsl", GL_FRAGMENT_SHADER);
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

	return true;
}

bool GraphicDevice::InitBuffers()
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

	return true;
}

bool GraphicDevice::InitLightBuffer()
{
	/* TEMP. Ta bort när ljus skapas från E/C-system*/
	int lights = 3;
	for (int i = 0; i < lights; i++)
	{
		testArray[10*i + 0] = -5.0+i*5;	//pos x
		testArray[10*i + 1] = 3.0;	//pos y
		testArray[10*i + 2] = 0.0;	//pos z
		testArray[10*i + 3] = 0.5;	 //int x
		testArray[10*i + 4] = 0.9;	 //int y
		testArray[10*i + 5] = 0.5;  //int z
		testArray[10*i + 6] = 0.8;	//col x
		testArray[10*i + 7] = 0.6;	//col y
		testArray[10*i + 8] = 0.6;	//col z
		testArray[10*i + 9] = 10.0;	//range
	}
	/* ---------------------------------------------- */
	glGenBuffers(1, &m_pointlightBuffer);

	if (m_pointlightBuffer < 0)
		return false;
	/* TMP  */
	float **tmpPointersArray = new float*[lights];
	for (int i = 0; i < lights; i++)
	{
		tmpPointersArray[i] = &testArray[10 * i];
	}
	BufferPointlights(lights, tmpPointersArray);
	delete tmpPointersArray;
	/* --------------------------------------------- */
	return true;
}

void GraphicDevice::BufferPointlights(int _nrOfLights, float **_lightPointers)
{
	m_vramUsage -= m_nrOfLights*10*sizeof(float);
	m_nrOfLights = _nrOfLights;

	float *pointlight_data = new float[_nrOfLights * 10];

	for (int i = 0; i < _nrOfLights; i++)
	{
		memcpy(&pointlight_data[10 * i], _lightPointers[i], 10 * sizeof(float));
	}

	int point_light_data_size = 10 * _nrOfLights * sizeof(float);

	m_compDeferredPass2Shader.UseProgram();

	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 4, m_pointlightBuffer, 0, point_light_data_size);
	glBufferData(GL_SHADER_STORAGE_BUFFER, point_light_data_size, pointlight_data, GL_STATIC_DRAW);
	m_vramUsage += m_nrOfLights*10*sizeof(float);

	delete pointlight_data;
}

bool GraphicDevice::InitTextRenderer()
{
	int texSizeX, texSizeY;
	GLuint m_textImage = AddTexture("content/textures/SimpleText.png", GL_TEXTURE20);
	return m_textRenderer.Init(m_textImage, m_clientWidth, m_clientHeight);
}
bool GraphicDevice::RenderSimpleText(std::string _text, int _x, int _y)
{
	return m_textRenderer.RenderSimpleText(_text, _x, _y);
}
void GraphicDevice::SetSimpleTextColor(vec4 _color)
{
	m_textRenderer.SetSimpleTextColor(_color);
}

bool GraphicDevice::PreLoadModel(std::string _dir, std::string _file, int _renderType)
{
	Shader *shaderPtr = NULL;
	if (_renderType == RENDER_DEFERRED)
	{
		shaderPtr = &m_deferredShader1;
		m_deferredShader1.UseProgram();
	}
	else if (_renderType == RENDER_FORWARD)
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
	}
	else
		ERRORMSG("ERROR: INVALID RENDER SETTING");

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

	Shader *shaderPtr = NULL;
	if (_renderType == RENDER_DEFERRED)
	{
		shaderPtr = &m_deferredShader1;
		m_deferredShader1.UseProgram();
	}
	else if (_renderType == RENDER_FORWARD)
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
	}
	else
		ERRORMSG("ERROR: INVALID RENDER SETTING");

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

	Model model = Model(modelID, mesh, texture, normal, specular);

	//for the matrices (modelView + normal)
	m_vramUsage += (16 + 9) * sizeof(float);

	if (_renderType == RENDER_DEFERRED)
	{
		for (int i = 0; i < m_modelsDeferred.size(); i++)
		{
			if (m_modelsDeferred[i] == model)
			{
				m_modelsDeferred[i].instances.push_back(Instance(modelID, true, _matrixPtr));
				return modelID;
			}
		}
	}
	else if (_renderType == RENDER_FORWARD)
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
	
	// Set model
	//if model doesnt exist
	model.instances.push_back(Instance(modelID, true, _matrixPtr));
	// Push back the model
	if (_renderType == RENDER_DEFERRED)
		m_modelsDeferred.push_back(model);
	else if (_renderType == RENDER_FORWARD)
		m_modelsForward.push_back(model);

	return modelID;
}
bool GraphicDevice::RemoveModel(int _id)
{
	for (int i = 0; i < m_modelsDeferred.size(); i++)
	{
		for (int j = 0; j < m_modelsDeferred.size(); j++)
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
		for (int j = 0; j < m_modelsForward.size(); j++)
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
	return false;
}
bool GraphicDevice::ActiveModel(int _id, bool _active)
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
	return false;
}
bool GraphicDevice::ChangeModelTexture(int _id, std::string _fileDir)
{
	//Här måste ändras! Skapa ny modell om man vill byta till en texturkombination som inte finns. Använd all återanvändbar data (mesh osv...) till den nya modellen.

	/*int location;
	GLuint texture;

	for (int i = 0; i < m_modelsDeferred.size(); i++)
	{
		for (int j = 0; j < m_modelsDeferred[i].instances.size(); j++)
		{
			if (m_modelsDeferred[i].instances[j].id == _id)
			{
				texture = AddTexture(_fileDir, GL_TEXTURE1);
				m_deferredShader1.CheckUniformLocation("diffuseTex", 1);

				m_modelsDeferred[i].texID = texture;

				return true;
			}
		}
	}
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].modelID == _id)
		{
			texture = AddTexture(_fileDir, GL_TEXTURE1);
			m_forwardShader.CheckUniformLocation("diffuseTex", 1);

			m_modelsForward[i].texID = texture;

			return true;
		}
	}*/
	return false;
}
bool GraphicDevice::ChangeModelNormalMap(int _id, std::string _fileDir)
{
	//Här måste ändras! Skapa ny modell om man vill byta till en texturkombination som inte finns. Använd all återanvändbar data (mesh osv...) till den nya modellen.
	/*int location;
	GLuint texture;

	for (int i = 0; i < m_modelsDeferred.size(); i++)
	{
		if (m_modelsDeferred[i].modelID == _id)
		{
			texture = AddTexture(_fileDir, GL_TEXTURE2);
			m_deferredShader1.CheckUniformLocation("normalTex", 2);

			m_modelsDeferred[i].norID = texture;

			return true;
		}
	}
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].modelID == _id)
		{
			texture = AddTexture(_fileDir, GL_TEXTURE2);
			m_forwardShader.CheckUniformLocation("normalTex", 2);

			m_modelsForward[i].norID = texture;

			return true;
		}
	}*/
	return false;
}
bool GraphicDevice::ChangeModelSpecularMap(int _id, std::string _fileDir)
{
	//Här måste ändras! Skapa ny modell om man vill byta till en texturkombination som inte finns. Använd all återanvändbar data (mesh osv...) till den nya modellen.
	/*int location;
	GLuint texture;

	for (int i = 0; i < m_modelsDeferred.size(); i++)
	{
		if (m_modelsDeferred[i].modelID == _id)
		{
			texture = AddTexture(_fileDir, GL_TEXTURE3);
			m_deferredShader1.CheckUniformLocation("specularTex", 3);

			m_modelsDeferred[i].speID = texture;

			return true;
		}
	}
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].modelID == _id)
		{
			texture = AddTexture(_fileDir, GL_TEXTURE3);
			m_forwardShader.CheckUniformLocation("specularTex", 3);

			m_modelsForward[i].speID = texture;

			return true;
		}
	}*/
	return false;
}

Buffer* GraphicDevice::AddMesh(std::string _fileDir, Shader *_shaderProg)
{
	for (std::map<const std::string, Buffer*>::iterator it = m_meshs.begin(); it != m_meshs.end(); it++)
	{		
		if (it->first == _fileDir)
			return it->second;
	}

	std::vector<Vertex> verts = ModelLoader::importMesh(_fileDir);

	std::vector<float> positionData(verts.size() * 3);
	std::vector<float> normalData(verts.size() * 3);
	std::vector<float> tanData(verts.size() * 3);
	std::vector<float> bitanData(verts.size() * 3);
	std::vector<float> texCoordData(verts.size() * 2);

	Buffer* retbuffer = new Buffer();

	_shaderProg->UseProgram();

	for (int i = 0; i < (int)verts.size(); i++)
	{
		positionData[i * 3 + 0] = verts[i].po.x;
		positionData[i * 3 + 1] = verts[i].po.y;
		positionData[i * 3 + 2] = verts[i].po.z;
		normalData[i * 3 + 0] = verts[i].no.x;
		normalData[i * 3 + 1] = verts[i].no.y;
		normalData[i * 3 + 2] = verts[i].no.z;
		tanData[i * 3 + 0] = verts[i].ta.x;
		tanData[i * 3 + 1] = verts[i].ta.y;
		tanData[i * 3 + 2] = verts[i].ta.z;
		bitanData[i * 3 + 0] = verts[i].bi.x;
		bitanData[i * 3 + 1] = verts[i].bi.y;
		bitanData[i * 3 + 2] = verts[i].bi.z;
		texCoordData[i * 2 + 0] = verts[i].uv.x;
		texCoordData[i * 2 + 1] = 1 - verts[i].uv.y;
	}

	BufferData bufferData[] =
	{
		{ 0, 3, GL_FLOAT, (const GLvoid*)positionData.data(), positionData.size() * sizeof(float) },
		{ 1, 3, GL_FLOAT, (const GLvoid*)normalData.data(), normalData.size()   * sizeof(float) },
		{ 2, 3, GL_FLOAT, (const GLvoid*)tanData.data(), tanData.size()   * sizeof(float) },
		{ 3, 3, GL_FLOAT, (const GLvoid*)bitanData.data(), bitanData.size()   * sizeof(float) },
		{ 4, 2, GL_FLOAT, (const GLvoid*)texCoordData.data(), texCoordData.size() * sizeof(float) }
	};

	int test = sizeof(bufferData) / sizeof(bufferData[0]);
	// Counts the size in bytes of all the buffered data
	for (int i = 0; i < sizeof(bufferData) / sizeof(bufferData[0]); i++)
		m_vramUsage += (int)bufferData[i].dataSize;


	retbuffer->init(bufferData, sizeof(bufferData) / sizeof(bufferData[0]));
	retbuffer->setCount((int)verts.size());
	
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
	m_deferredShader1.UseProgram();
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
  
  m_modelsDeferred.clear();
  m_modelsForward.clear();
}
