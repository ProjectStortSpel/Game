#ifndef GRAPHICDEVICE_H
#define GRAPHICDEVICE_H

#define GLEW_STATIC

#include <SDL/SDL.h>
#include <GLEW/glew.h>
#include <SDL/SDL_opengl.h>
#include <string>
#include <vector>

namespace Renderer
{
	struct RenderComponent
	{


	};

	class DECLSPEC GraphicDevice
	{
	public:
		GraphicDevice();
		~GraphicDevice();

		void Init();

		void PollEvent(SDL_Event _event);
		void Update(float _dt);
		void Render();

		void ResizeWindow(int _width, int _height);
		void SetTitle(std::string _title);

	private:
		bool InitSDLWindow();
		bool InitDeferred();
		
		SDL_Window*		m_window;
		SDL_GLContext	m_glContext;

		// Window size
		int	m_clientWidth, m_clientHeight;
	};
}

#endif