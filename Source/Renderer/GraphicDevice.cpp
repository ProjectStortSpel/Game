#include "GraphicDevice.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbimage/stb_image.h>

using namespace Renderer;

GraphicDevice::GraphicDevice()
{
}

GraphicDevice::~GraphicDevice()
{
}

bool GraphicDevice::Init()
{
	if (!InitSDLWindow()) { ERRORMSG("INIT SDL WINDOW FAILED\n"); return false; }
	if (!InitGLEW()) { ERRORMSG("GLEW_VERSION_4_3 FAILED\n"); return false; }
	if (!InitDeferred()) { ERRORMSG("INIT DEFERRED FAILED\n"); return false; }
	if (!InitShaders()) { ERRORMSG("INIT SHADERS FAILED\n"); return false; }
	if (!InitBuffers()) { ERRORMSG("INIT BUFFERS FAILED\n"); return false; }

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

}

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
	// FORWARD RENDER
	// POST RENDER EFFECTS?
	// GUI RENDER

	// DEBUGG TEXT


	//glBindTexture(GL_TEXTURE_2D, m_debuggText);
	// Use Debuggtext
	for (int i = 0; i < 4608; i++)
	{
		debugtext[i] = rand() % 4608;
	}

	glBindBuffer(GL_ARRAY_BUFFER, debugtextbuffer);
	GLvoid* p = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &debugtext, sizeof(int) * 4608);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glUseProgram(m_debuggTextShader.GetShaderProgram());
	// Run program
	glDispatchCompute(128, 36, 1); // 1/16 = 0.0625



	// FULL SCREEN QUAD
	glBindTexture(GL_TEXTURE_2D, m_outputImage);
	// Clear, select the rendering program and draw a full screen quad	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_fullScreenShader.GetShaderProgram());
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
	int				PosX = 100;
	int				PosY = 100;
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
	if (!glewIsSupported("GLEW_VERSION_4_3")) { return false; }
#endif

	SDL_GL_SetSwapInterval(0);
	
	return true;
}

bool GraphicDevice::InitDeferred()
{
	return true;
}

bool GraphicDevice::InitShaders()
{
	// debuggtext Shader
	ShaderInfo shaderInfo[] =
	{
		{ GL_COMPUTE_SHADER, "content/shaders/debuggText.glsl" },
		{ GL_NONE, NULL },
		{ GL_NONE, NULL }
	};
	m_debuggTextShader.Init(shaderInfo);

	// Full Screen Quad Shader
	ShaderInfo shaderInfo9[] =
	{
		{ GL_VERTEX_SHADER, "content/shaders/fullscreen.vs" },
		{ GL_GEOMETRY_SHADER, "content/shaders/fullscreen.gs" },
		{ GL_FRAGMENT_SHADER, "content/shaders/fullscreen.ps" }
	};
	m_fullScreenShader.Init(shaderInfo9);

	return true;
}

bool GraphicDevice::InitBuffers()
{
	

	// OutputImageBuffer
	glGenTextures(1, &m_outputImage);
	glBindTexture(GL_TEXTURE_2D, m_outputImage);
	glTexStorage2D(GL_TEXTURE_2D, 8, GL_RGBA32F, m_clientWidth, m_clientHeight);

	glBindImageTexture(0, m_outputImage, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	// load img test kod

	for (int i = 0; i < 4608; i++)
	{
		float x = (float)(i%128) / 128;
		float y = (float)(i/128) / 36;
		debugtext[i] = i;
	}

	glGenBuffers(1, &debugtextbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, debugtextbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int)* 4608, &debugtext, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, debugtextbuffer);


	return true;
}

void GraphicDevice::LoadTexture(string file)
{
	int w;
	int h;
	int comp;
	unsigned char* image = stbi_load(file.c_str(), &w, &h, &comp, STBI_rgb_alpha);

	if (image == nullptr)
		return;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image);
}