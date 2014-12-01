/*
Author: Anders, Christian
*/
#ifndef GRAPHICDEVICE_H
#define GRAPHICDEVICE_H

#include "stdafx.h"
#include "Shader.h"
#include "SimpleText.h"
#include "GLTimer.h"
#include "Camera.h""

namespace Renderer
{
	struct Model
	{
		bool operator> (const Model &m) { return VAOHandle > m.VAOHandle ? true : false; }
		bool operator< (const Model &m) { return VAOHandle < m.VAOHandle ? true : false; }

		bool operator== (const Model &m) { return VAOHandle == m.VAOHandle ? true : false; }
		bool operator!= (const Model &m) { return VAOHandle == m.VAOHandle ? false : true; }

		Model(){}
		Model(GLuint vao, GLuint tex, GLuint nor, GLuint spe)
		{
			VAOHandle = vao;
			texID = tex;
			norID = nor;
			speID = spe;
			modelMatrix = glm::translate(glm::vec3(1));
		}
		GLuint VAOHandle;
		GLuint texID;
		GLuint norID;
		GLuint speID;

		glm::vec3 position;
		float rotation;
		glm::mat4 modelMatrix;

		void Update()
		{
			modelMatrix = glm::translate(position) * glm::rotate(glm::mat4(1.0), rotation, glm::vec3(0.0, 1.0, 0.0));
		}
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

		Camera *GetCamera(){ return m_camera; }
		void GetWindowSize(int &x, int &y){ x = m_clientWidth; y = m_clientHeight; }

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

		Camera* m_camera;

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
		std::map<const std::string, GLuint> m_meshs;
		GLuint AddMesh(std::string _fileDir);
		// Textures
		std::map<const std::string, GLuint> m_textures;
		GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);
	};
}



#endif