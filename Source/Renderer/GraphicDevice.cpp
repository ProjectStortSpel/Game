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

void GraphicDevice::Init()
{
	if (!InitSDLWindow()) std::cout << "INIT SDL WINDOW FAILED!" << std::endl;
	m_glContext = SDL_GL_CreateContext(m_window);

	glewInit();
	SDL_GL_SetSwapInterval(0);

	if (!InitDeferred()) std::cout << "INIT DEFERRED FAILED!" << std::endl;

	if (!InitShaders()) std::cout << "INIT SHADERS FAILED!" << std::endl;

	if (!InitBuffers()) std::cout << "INIT BUFFERS FAILED!" << std::endl;




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
	glUseProgram(m_debuggTextShader.GetShaderProgram());
	// Run program
	glDispatchCompute(m_clientWidth * 0.0625, m_clientHeight * 0.0625, 1); // 1/16 = 0.0625

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