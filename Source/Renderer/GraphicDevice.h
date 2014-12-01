/*
Author: Anders, Christian
*/
#ifndef GRAPHICDEVICE_H
#define GRAPHICDEVICE_H

#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"
#include "SimpleText.h"
#include "GLTimer.h"
#include "Camera.h""

namespace Renderer
{
	struct Model
	{
		bool operator> (const Model &m) { return bufferPtr->getVAO() > m.bufferPtr->getVAO() ? true : false; }
		bool operator< (const Model &m) { return bufferPtr->getVAO() < m.bufferPtr->getVAO() ? true : false; }

		bool operator== (const Model &m) { return bufferPtr->getVAO() == m.bufferPtr->getVAO() ? true : false; }
		bool operator!= (const Model &m) { return bufferPtr->getVAO() == m.bufferPtr->getVAO() ? false : true; }

		Model(){}
		Model(Buffer* buffer, GLuint tex, GLuint nor, GLuint spe)
		{
			bufferPtr = buffer;
			texID = tex;
			norID = nor;
			speID = spe;
			modelMatrix = glm::translate(glm::vec3(1));
		}
		Buffer* bufferPtr;
		GLuint texID;
		GLuint norID;
		GLuint speID;

		glm::mat4 modelMatrix;	// GÖR DETTA TILL EN PEKARE NÄR E/C FUNGERAR
	};


	struct GLTimerValue
	{
		std::string name;
		float ms;
		GLTimerValue(std::string n, float m)
		{
			name = n;
			ms = m;
		}
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

		// SIMPLETEXT FROM GAME
		bool RenderSimpleText(std::string _text, int x, int y);
		void SetSimpleTextColor(glm::vec4 _color);

		// MODELLOADER
		void LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr);

	private:
		bool InitSDLWindow();
		bool InitGLEW();
		bool InitDeferred();
		bool InitShaders();
		bool InitBuffers();
		bool InitTextRenderer();

		void CreateGBufTex(GLenum texUnit, GLenum format, GLuint &texid);
		void CreateDepthTex(GLuint &texid);

		SDL_Window*		m_window;
		SDL_GLContext	m_glContext;

		// dt and fps
		float m_dt;
		int m_fps;

		// Timer for shader run time
		std::vector<GLTimerValue> m_glTimerValues;

		// Window size
		int	m_clientWidth, m_clientHeight;

		// Image buffers
		GLuint m_outputImage, m_inputImage;
		GLuint m_debuggText;
		GLuint m_depthBuf, m_normTex, m_colorTex;

		// Frame buffer object
		GLuint m_deferredFBO;

		// Shaders
		Shader m_debuggTextShader; // TA BORT DENNA
		Shader m_fullScreenShader;
		Shader m_deferredShader1, m_compDeferredPass2Shader;

		// SimpleText
		SimpleText m_textRenderer;


		// Modelloader
		std::vector<Model> m_models;

		// Meshs
		std::map<const std::string, Buffer*> m_meshs;
		Buffer* AddMesh(std::string _fileDir);
		// Textures
		std::map<const std::string, GLuint> m_textures;
		GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);
	};
}



#endif