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
#include "Camera.h"
#include "SkyBox.h"
#include "ShadowMap.h"

namespace Renderer
{
#define RENDER_DEFERRED 0
#define RENDER_FORWARD  1

#define TEXTURE_DIFFUSE		0
#define TEXTURE_NORMAL		1
#define TEXTURE_SPECULAR	2


	struct Instance
	{
		int id;
		bool active;
		mat4* modelMatrix;
		
		Instance(){}
		Instance(int _id, bool _active, mat4* _model)
		{
			id = _id;
			active = _active;
			modelMatrix = _model;
		}
	};

	struct Model
	{
		bool operator== (const Model &m) { return Compare(m); }
		bool operator!= (const Model &m) { return !Compare(m); }

		Model(){}
		Model(Buffer* buffer, GLuint tex, GLuint nor, GLuint spe)
		{
			bufferPtr = buffer;
			texID = tex;
			norID = nor;
			speID = spe;
		}
		bool Compare(Model m)
		{
			if (texID != m.texID) return false;
			if (bufferPtr != m.bufferPtr) return false;
			if (speID != m.speID) return false;
			if (norID != m.norID) return false;
			return true;
		}
		Buffer* bufferPtr;
		GLuint texID;
		GLuint norID;
		GLuint speID;

		std::vector<Instance> instances;
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
		void SetSimpleTextColor(float _r, float _g, float _b, float _a);
		void SetDisco();
		void ToggleSimpleText();
		void ToggleSimpleText(bool _on);

		Camera *GetCamera(){ return m_camera; }
		void GetWindowSize(int &x, int &y){ x = m_clientWidth; y = m_clientHeight; }

		// MODELLOADER
		bool PreLoadModel(std::string _dir, std::string _file, int _renderType = RENDER_DEFERRED);
		int LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_DEFERRED);
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);
		bool ChangeModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE);
		bool ChangeModelNormalMap(int _id, std::string _fileDir);
		bool ChangeModelSpecularMap(int _id, std::string _fileDir);

		void SetDebugTexFlag(int _flag) { m_debugTexFlag = _flag; }
		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);
		
		void Clear();

	private:
		bool InitSDLWindow();
		bool InitGLEW();
		bool InitDeferred();
		bool InitForward();
		bool InitShaders();
		bool InitBuffers();
		bool InitTextRenderer();
		bool InitLightBuffers();
		bool InitSkybox();
		bool InitRandomVector();

		void CreateGBufTex(GLenum texUnit, GLenum format, GLuint &texid);
		void CreateDepthTex(GLuint &texid);

		void CreateShadowMap();

		Camera* m_camera;

		SDL_Window*		m_window;
		SDL_GLContext	m_glContext;

		// dt and fps
		float m_dt;
		int m_fps;

		int m_vramUsage; //in bytes

		//holds the reset values for lights
		float m_lightDefaults[19];

		//Le shadowmap
		ShadowMap *m_shadowMap;
		void WriteShadowMapDepth();

		// Timer for shader run time
		std::vector<GLTimerValue> m_glTimerValues;

		// Window size
		int	m_clientWidth, m_clientHeight;

		// Image buffers
		//GLuint m_skyBox;
		GLuint m_outputImage;
		GLuint m_debuggText;
		GLuint m_depthBuf, m_normTex, m_colorTex;

		// Frame buffer object
		GLuint m_deferredFBO, m_forwardFBO;

		// Shaders
		Shader m_fullScreenShader;
		Shader m_skyBoxShader;
		Shader m_deferredShader1, m_compDeferredPass2Shader;
		Shader m_forwardShader;

		Shader m_shadowShaderDeferred, m_shadowShaderForward;

		// Skybox
		SkyBox *m_skybox;

		// SimpleText
		bool m_renderSimpleText;
		SimpleText m_textRenderer;

		// Modelloader
		int m_modelIDcounter;
		std::vector<Model> m_modelsDeferred, m_modelsForward;

		// Pointlights buffer
		GLuint m_pointlightBuffer, m_dirLightBuffer;
		vec3 m_dirLightDirection;

		// DEBUG variables ----
		int m_debugTexFlag; // 0=standard, 1=diffuse, 2=normal, 3=specular+shine, 4=glow
		int m_nrOfLights; // lol

		// Objects
		//std::map<const std::string, ObjectData> m_objects;
		//class ObjectData AddObject(std::string _file, std::string _dir);
		// Meshs
		std::map<const std::string, Buffer*> m_meshs;
		Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg);
		// Textures
		std::map<const std::string, GLuint> m_textures;
		GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);

		// Random Vertors
		GLuint m_randomVectors;
	};
}



#endif