#include "GraphicDevice.h"

#define STB_IMAGE_IMPLEMENTATION
#include "TextureLoader.h"

using glm::vec3;
GLuint	VAOFullscreenQuad;

using namespace Renderer;

struct Object
{
	Object(){}
	Object(GLuint tex, glm::vec3 pos)
	{
		texID = tex;
		position = pos;
		modelMatrix = glm::translate(pos);
	}
	GLuint texID, VAOHandle;
	glm::vec3 position;
	float rotation;
	glm::mat4 modelMatrix;
	void Update()
	{
		modelMatrix = glm::translate(position) * glm::rotate(glm::mat4(1.0), rotation, vec3(0.0, 1.0, 0.0));
	}
};

Object m_Quad, m_Ground;

//---------TEMP------------------------
void CreateDrawQuad()
{
	float positionData[] = {
		-1.0, -1.0,
		1.0, -1.0,
		1.0, 1.0,
		1.0, 1.0,
		-1.0, 1.0,
		-1.0, -1.0
	};

	//drawShaderHandle.UseProgram();

	GLuint VBOHandle;
	glGenBuffers(1, &VBOHandle);

	glGenVertexArrays(1, &VAOFullscreenQuad);
	glBindVertexArray(VAOFullscreenQuad);

	glBindBuffer(GL_ARRAY_BUFFER, VBOHandle);
	glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), positionData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandle);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
}

void CreateQuad()
{
	float positionData[] = {
		-1.0, -1.0, 0.0,
		1.0, 1.0, 0.0,
		-1.0, 1.0, 0.0,
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
		1.0, 1.0, 0.0
	};

	float normalData[] = {
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0
	};

	float texCoordData[] = {
		0.0, 1 - 0.0,
		1.0, 1 - 1.0,
		0.0, 1 - 1.0,
		0.0, 1 - 0.0,
		1.0, 1 - 0.0,
		1.0, 1 - 1.0
	};

	//drawShaderHandle.UseProgram();

	GLuint VAOHandle;
	GLuint VBOHandles[3];
	glGenBuffers(3, VBOHandles);


	// "Bind" (switch focus to) first buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), normalData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[2]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), texCoordData, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAOHandle);
	glBindVertexArray(VAOHandle);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // normal
	glEnableVertexAttribArray(2); // texCoord

	// map indices to buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	m_Quad.VAOHandle = VAOHandle;
}

void CreateGround()
{
	float positionData[] = {
		-5.0, 0.0, 5.0,
		5.0, 0.0, -5.0,
		-5.0, 0.0, -5.0,
		-5.0, 0.0, 5.0,
		5.0, 0.0, 5.0,
		5.0, 0.0, -5.0
	};

	float normalData[] = {
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0
	};

	float texCoordData[] = {
		0.0, 1 - 0.0,
		1.0, 1 - 1.0,
		0.0, 1 - 1.0,
		0.0, 1 - 0.0,
		1.0, 1 - 0.0,
		1.0, 1 - 1.0
	};

	GLuint VAOGround;
	GLuint VBOHandles[3];
	glGenBuffers(3, VBOHandles);

	// "Bind" (switch focus to) first buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), normalData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[2]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), texCoordData, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAOGround);
	glBindVertexArray(VAOGround);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // normal
	glEnableVertexAttribArray(2); // texCoord

	// map indices to buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, VBOHandles[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	m_Ground.VAOHandle = VAOGround;
}

//-------------------------------------

GraphicDevice::GraphicDevice()
{
}

GraphicDevice::~GraphicDevice()
{
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
	//if (!InitTextRenderer()) { ERRORMSG("INIT TEXTRENDERER FAILED\n"); return false; }

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

	//------Render deferred--------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_deferredFBO);
	glViewport(0, 0, m_clientWidth, m_clientHeight);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.15, 1.0f);

	m_deferredShader1.UseProgram();
	rot += 0.05f;
	//--------Uniforms-------------------------------------------------------------------------
	glm::mat4 projectionMatrix = glm::perspective(45.0f, (float)m_clientWidth / (float)m_clientHeight, 0.2f, 100.f);
	m_deferredShader1.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	//----------------------------------------------------------------------------------------

	glm::mat4 viewMatrix = glm::lookAt(
		vec3(0.0, 0.5, 4.0),	//*m_cam->Get_pos(), // the position of your camera, in world space
		vec3(0.0, 0.0, 0.0),   // where you want to look at, in world space
		vec3(0.0, 1.0, 0.0)		 // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
		);

	//Render scene
	//--Object 1--------
	m_Quad.rotation = rot;
	m_Quad.Update();
	glm::mat4 modelMatrix = m_Quad.modelMatrix;
	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));

	m_deferredShader1.SetUniVariable("ModelViewMatrix", mat4x4, &modelViewMatrix);
	m_deferredShader1.SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_Quad.texID);
	glBindVertexArray(m_Quad.VAOHandle);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);

	//---Object 2------------
	modelMatrix = m_Ground.modelMatrix;
	modelViewMatrix = viewMatrix * modelMatrix;
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));

	m_deferredShader1.SetUniVariable("ModelViewMatrix", mat4x4, &modelViewMatrix);
	m_deferredShader1.SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);
	
	glBindTexture(GL_TEXTURE_2D, m_Ground.texID);
	glBindVertexArray(m_Ground.VAOHandle);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);


	//-----Pass2------------------
/*	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	m_deferredShader2.UseProgram();
	m_deferredShader2.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);

	glm::mat4 inverseProjection = glm::inverse(projectionMatrix);
	m_deferredShader2.SetUniVariable("invProjection", mat4x4, &inverseProjection);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_depthBuf);
	//Full screen quad to draw to
	glBindVertexArray(VAOFullscreenQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//---------------------------
	glActiveTexture(0);
*/

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

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_depthBuf);

	glDispatchCompute(m_clientWidth * 0.0625, m_clientHeight * 0.0625, 1); // 1/16 = 0.0625
	//---------------------------------------------------------------------------

	//m_textRenderer.RenderText();
	
	// FULL SCREEN QUAD
	m_fullScreenShader.UseProgram();
	glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, m_outputImage);
	glDrawArrays(GL_POINTS, 0, 1);

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
	int				SizeX = 256 * 4;
	int				SizeY = 144 * 4;

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

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);
	return true;
}

bool GraphicDevice::InitShaders()
{
	// compute Shader
	m_compDeferredPass2Shader.InitShaderProgram();
	m_compDeferredPass2Shader.AddShader("Content/Shaders/CSDeferredPass2.glsl", GL_COMPUTE_SHADER);
	m_compDeferredPass2Shader.FinalizeShaderProgram();

	// Full Screen Quad Shader
	m_fullScreenShader.InitShaderProgram();
	m_fullScreenShader.AddShader("content/shaders/fullscreen.vs", GL_VERTEX_SHADER);
	m_fullScreenShader.AddShader("content/shaders/fullscreen.gs", GL_GEOMETRY_SHADER);
	m_fullScreenShader.AddShader("content/shaders/fullscreen.ps", GL_FRAGMENT_SHADER);
	m_fullScreenShader.FinalizeShaderProgram();

	//Deferred pass 1
	m_deferredShader1.InitShaderProgram();
	m_deferredShader1.AddShader("Content/Shaders/VSDeferredPass1.glsl", GL_VERTEX_SHADER);
	m_deferredShader1.AddShader("Content/Shaders/FSDeferredPass1.glsl", GL_FRAGMENT_SHADER);
	m_deferredShader1.FinalizeShaderProgram();

	//Deferred pass 2
	/*m_deferredShader2.InitShaderProgram();
	m_deferredShader2.AddShader("Content/Shaders/VSDeferredPass2.glsl", GL_VERTEX_SHADER);
	m_deferredShader2.AddShader("Content/Shaders/FSDeferredPass2.glsl", GL_FRAGMENT_SHADER);
	m_deferredShader2.FinalizeShaderProgram();*/

	return true;
}

bool GraphicDevice::InitBuffers()
{
	int location;

	m_compDeferredPass2Shader.UseProgram();

	// Compute shader input images
	glActiveTexture(GL_TEXTURE0);
	//normal
	glBindImageTexture(0, m_normTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);


	glActiveTexture(GL_TEXTURE1);
	//color
	glBindImageTexture(1, m_colorTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);


	glActiveTexture(GL_TEXTURE2);
	location = glGetUniformLocation(m_compDeferredPass2Shader.GetShaderProgram(), "DepthTex");
	glUniform1i(location, 2);

	
	// Output ImageBuffer
	glGenTextures(1, &m_outputImage);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_outputImage);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, m_clientWidth, m_clientHeight);

	glBindImageTexture(5, m_outputImage, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	m_fullScreenShader.UseProgram();
	glActiveTexture(GL_TEXTURE5);
	location = glGetUniformLocation(m_fullScreenShader.GetShaderProgram(), "output_image");
	glUniform1i(location, 5);

	//m_deferredShader2.UseProgram();
	//CreateDrawQuad();

	
	m_deferredShader1.UseProgram();
	GLuint texture = TextureLoader::LoadTexture("Content/Textures/tiles.png", GL_TEXTURE3);
	glActiveTexture(GL_TEXTURE3);
	location = glGetUniformLocation(m_deferredShader1.GetShaderProgram(), "diffuseTex");
	glUniform1i(location, 3);

	m_Quad = Object(texture, vec3(0.0));
	CreateQuad();

	texture = TextureLoader::LoadTexture("Content/Textures/floor.png", GL_TEXTURE3);
	glActiveTexture(GL_TEXTURE3);
	m_Ground = Object(texture, vec3(0.0, -1.5, 0.0));
	CreateGround();

	m_fullScreenShader.UseProgram();
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_outputImage);
	location = glGetUniformLocation(m_fullScreenShader.GetShaderProgram(), "output_image");
	glUniform1i(location, 5);

	return true;
}

bool GraphicDevice::InitTextRenderer()
{
	GLuint m_textImage = TextureLoader::LoadTexture("content/textures/SimpleText.png", 5);
	return m_textRenderer.Init(&m_outputImage, m_textImage, m_clientWidth, m_clientHeight);
}
bool GraphicDevice::RenderSimpleText(std::string _text, int _x, int _y)
{
	return m_textRenderer.RenderSimpleText(_text, _x, _y);
}
void GraphicDevice::SetSimpleTextColor(vec4 _color)
{
	m_textRenderer.SetSimpleTextColor(_color);
}