#ifndef DEBUGTEXT_H
#define DEBUGTEXT_H

#include "stdafx.h"
#include "Shader.h"

namespace Renderer
{
	struct RenderComponent
	{
		int letter;
	};

	class DebugText
	{
	public:
		DebugText();
		~DebugText();

		bool Init();

		void PollEvent(SDL_Event _event);
		void Update(float _dt);
		void Render();

		void ResizeWindow(int _width, int _height);
		void SetTitle(std::string _title);

	private:
		bool InitSDLWindow();
		bool InitGLEW();
		bool InitDeferred();
		bool InitShaders();
		bool InitBuffers();

		void LoadTexture(string file);

		SDL_Window*		m_window;
		SDL_GLContext	m_glContext;

		// Window size
		int	m_clientWidth, m_clientHeight;

		// Image buffers
		GLuint m_outputImage;
		GLuint m_debuggText;

		// Shaders
		Shader m_compDeferredPass2Shader;
		Shader m_fullScreenShader;
	};
}

#endif