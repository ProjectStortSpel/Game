#include "GraphicDevice.h"
#include <iostream>
#include <stdio.h>
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

}

void GraphicDevice::ResizeWindow(int _width, int _height)
{
	std::cout << "Resize: " << _width << "x" << _height << std::endl;
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