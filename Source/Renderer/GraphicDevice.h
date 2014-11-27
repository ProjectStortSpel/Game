/*
Author: Anders, Christian
*/
#ifndef GRAPHICDEVICE_H
#define GRAPHICDEVICE_H

#include "stdafx.h"
#include "Shader.h"

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

		void CreateGBufTex(GLenum texUnit, GLenum format, GLuint &texid);
		void CreateDepthTex(GLuint &texid);

		SDL_Window*		m_window;
		SDL_GLContext	m_glContext;

		// Window size
		int	m_clientWidth, m_clientHeight;

		// Image buffers
		GLuint m_outputImage, m_inputImage;
		GLuint m_debuggText;
		GLuint m_depthBuf, m_normTex, m_colorTex;

		// Frame buffer object
		GLuint m_deferredFBO;

		// Shaders
		Shader m_compDeferredPass2Shader;
		Shader m_fullScreenShader;
		Shader m_deferredShader1, m_deferredShader2;
		int debugtext[4608];
		GLuint debugtextbuffer;
	};
}

#endif