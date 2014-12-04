#include "GraphicDevice.h"

#define STB_IMAGE_IMPLEMENTATION
#include "TextureLoader.h"
#include "ModelLoader.h"

using namespace Renderer;
using namespace glm;

GraphicDevice::GraphicDevice()
{
	m_modelIDcounter = 0;
	m_camera = new Camera();
	m_vramUsage = 0;
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
	if (!InitTextRenderer()) { ERRORMSG("INIT TEXTRENDERER FAILED\n"); return false; }
	m_vramUsage += (m_textRenderer.GetArraySize() * sizeof(int));

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

float rot = 0.0f;
void GraphicDevice::Render()
{
	// DEFERRED RENDER STEPS
	/*
	(- Step0	Render shadowmaps)

	- Step1	SETUP
		- Output	normal + spec int
					albedo + spec pow
					depth

		- Input		View
					Projection

	DRAW STUFF HERE

	- Step2	PROCESS
		- Input		normal + spec int
					depth
					random map

		- Output
					difflight + ssao
					speclight

	- Step3 FINISH
		- Input		albedo + specpow
					difflight + ssao
					speclight

	- Output	color
	*/
		//GLTimer glTimer;
		//glTimer.Start();
	//------Render deferred--------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_deferredFBO);
	glViewport(0, 0, m_clientWidth, m_clientHeight);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.15, 1.0f);

	m_deferredShader1.UseProgram();
	rot += m_dt;
	//--------Uniforms-------------------------------------------------------------------------
	mat4 projectionMatrix = glm::perspective(45.0f, (float)m_clientWidth / (float)m_clientHeight, 0.2f, 100.f);
	m_deferredShader1.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	//----------------------------------------------------------------------------------------

	glm::mat4 viewMatrix = *m_camera->GetViewMatrix();

	//------Render scene--------------------------------------------------------------
	
	//-- DRAW MODELS
	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i].active) // IS MODEL ACTIVE?
		{
			glm::mat4 modelMatrix;
			if (m_models[i].modelMatrix == NULL)
				modelMatrix = glm::translate(glm::vec3(1));
			else
				modelMatrix = *m_models[i].modelMatrix;

			glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));

			m_deferredShader1.SetUniVariable("ModelViewMatrix", mat4x4, &modelViewMatrix);
			m_deferredShader1.SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_models[i].texID);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_models[i].norID);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_models[i].speID);

			m_models[i].bufferPtr->draw();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	

		//m_glTimerValues.push_back(GLTimerValue("Deferred stage1: ", glTimer.Stop()));
		//glTimer.Start();
	// FORWARD RENDER
	// POST RENDER EFFECTS?
	// GUI RENDER

	// DEBUGG TEXT
	//glBindTexture(GL_TEXTURE_2D, m_debuggText);
	// Use Debuggtext
	
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	//----Compute shader (pass 2)------------------------------------------------
	m_compDeferredPass2Shader.UseProgram();

	m_compDeferredPass2Shader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);
	glm::mat4 inverseProjection = glm::inverse(projectionMatrix);
	m_compDeferredPass2Shader.SetUniVariable("invProjection", mat4x4, &inverseProjection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_depthBuf);

	glBindImageTexture(1, m_colorTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glBindImageTexture(2, m_randomVectors, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

	glDispatchCompute(m_clientWidth * 0.0625, m_clientHeight * 0.0625, 1); // 1/16 = 0.0625
	//---------------------------------------------------------------------------

		//m_glTimerValues.push_back(GLTimerValue("Deferred stage2: ", glTimer.Stop()));
		//glTimer.Start();

	m_textRenderer.RenderText(m_dt);

		//m_glTimerValues.push_back(GLTimerValue("Text Render: ", glTimer.Stop()));
		//glTimer.Start();

	// FULL SCREEN QUAD
	m_fullScreenShader.UseProgram();
	glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, m_outputImage);
	glDrawArrays(GL_POINTS, 0, 1);

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
	int				PosX = 200;
	int				PosY = 280;
	int				SizeX = 256 * 5;
	int				SizeY = 144 * 5;

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

	m_vramUsage += ( m_clientWidth*m_clientHeight*sizeof(float) );
	m_vramUsage += ( m_clientWidth*m_clientHeight*sizeof(float) * 4 * 2);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);

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

	// Full Screen Quad
	m_fullScreenShader.InitShaderProgram();
	m_fullScreenShader.AddShader("content/shaders/fullscreen.vs", GL_VERTEX_SHADER);
	m_fullScreenShader.AddShader("content/shaders/fullscreen.gs", GL_GEOMETRY_SHADER);
	m_fullScreenShader.AddShader("content/shaders/fullscreen.ps", GL_FRAGMENT_SHADER);
	m_fullScreenShader.FinalizeShaderProgram();

	return true;
}

bool GraphicDevice::InitBuffers()
{
	int location;

	m_compDeferredPass2Shader.UseProgram();

	// Compute shader input images
	//normal
	glBindImageTexture(0, m_normTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

	//color
	glBindImageTexture(1, m_colorTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

	// Create random vector img for SSAO
	m_randomVectors = AddTexture("content/textures/vectormap.png", GL_TEXTURE21);
	glBindImageTexture(2, m_randomVectors, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);


	glActiveTexture(GL_TEXTURE0);
	location = glGetUniformLocation(m_compDeferredPass2Shader.GetShaderProgram(), "DepthTex");
	glUniform1i(location, 0);
	

	// Output ImageBuffer
	glGenTextures(1, &m_outputImage);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_outputImage);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, m_clientWidth, m_clientHeight);

	glBindImageTexture(5, m_outputImage, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	// FULL SCREEN QUAD
	m_fullScreenShader.UseProgram();
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_outputImage);
	location = glGetUniformLocation(m_fullScreenShader.GetShaderProgram(), "output_image");
	glUniform1i(location, 5);


	return true;
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


int GraphicDevice::LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr)
{
	int location;
	int modelID = m_modelIDcounter;
	m_modelIDcounter++;

	// Import Object
	ObjectData obj = ModelLoader::importObject(_dir, _file);

	// Import Texture
	GLuint texture = AddTexture(obj.text, GL_TEXTURE1);
	glActiveTexture(GL_TEXTURE1);
	location = glGetUniformLocation(m_deferredShader1.GetShaderProgram(), "diffuseTex");
	glUniform1i(location, 1);

	// Import Normal map
	GLuint normal = AddTexture(obj.norm, GL_TEXTURE2);
	glActiveTexture(GL_TEXTURE2);
	location = glGetUniformLocation(m_deferredShader1.GetShaderProgram(), "normalTex");
	glUniform1i(location, 2);

	// Import Specc Glow map
	GLuint specular = AddTexture(obj.spec, GL_TEXTURE3);
	glActiveTexture(GL_TEXTURE3);
	location = glGetUniformLocation(m_deferredShader1.GetShaderProgram(), "specularTex");
	glUniform1i(location, 3);

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh);

	// Set model
	Model model = Model(modelID, mesh, texture, normal, specular);
	
	model.modelMatrix = _matrixPtr;
	// Push back the model
	m_models.push_back(model);
	std::push_heap(m_models.begin(), m_models.end());

	return modelID;
}
bool GraphicDevice::RemoveModel(int _id)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i].modelID == _id)
		{
			m_models.erase(m_models.begin() + i);
			return true;
		}
	}
	return false;
}
bool GraphicDevice::ActiveModel(int _id, bool _active)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i].modelID == _id)
		{
			m_models[i].active = _active;
			return true;
		}
	}
	return false;
}
bool GraphicDevice::ChangeModelTexture(int _id, std::string _fileDir)
{
	int location;
	GLuint texture;

	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i].modelID == _id)
		{
			texture = AddTexture(_fileDir, GL_TEXTURE1);
			glActiveTexture(GL_TEXTURE1);
			location = glGetUniformLocation(m_deferredShader1.GetShaderProgram(), "diffuseTex");
			glUniform1i(location, 1);
			
			m_models[i].texID = texture;

			return true;
		}
	}
	return false;
}
bool GraphicDevice::ChangeModelNormalMap(int _id, std::string _fileDir)
{
	int location;
	GLuint texture;

	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i].modelID == _id)
		{
			texture = AddTexture(_fileDir, GL_TEXTURE2);
			glActiveTexture(GL_TEXTURE2);
			location = glGetUniformLocation(m_deferredShader1.GetShaderProgram(), "normalTex");
			glUniform1i(location, 2);

			m_models[i].texID = texture;

			return true;
		}
	}
	return false;
}
bool GraphicDevice::ChangeModelSpecularMap(int _id, std::string _fileDir)
{
	int location;
	GLuint texture;

	for (int i = 0; i < m_models.size(); i++)
	{
		if (m_models[i].modelID == _id)
		{
			texture = AddTexture(_fileDir, GL_TEXTURE3);
			glActiveTexture(GL_TEXTURE3);
			location = glGetUniformLocation(m_deferredShader1.GetShaderProgram(), "specularTex");
			glUniform1i(location, 3);

			m_models[i].texID = texture;

			return true;
		}
	}
	return false;
}


Buffer* GraphicDevice::AddMesh(std::string _fileDir)
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

	m_vramUsage += ( 14 * (int)verts.size() * sizeof(float) );

	Buffer* retbuffer = new Buffer();

	BufferData bufferData[] =
	{
		{ 0, 3, GL_FLOAT, (const GLvoid*)positionData.data(), positionData.size() * sizeof(float) },
		{ 1, 3, GL_FLOAT, (const GLvoid*)normalData.data(), normalData.size()   * sizeof(float) },
		{ 2, 3, GL_FLOAT, (const GLvoid*)tanData.data(), tanData.size()   * sizeof(float) },
		{ 3, 3, GL_FLOAT, (const GLvoid*)bitanData.data(), bitanData.size()   * sizeof(float) },
		{ 4, 2, GL_FLOAT, (const GLvoid*)texCoordData.data(), texCoordData.size() * sizeof(float) }
	};
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