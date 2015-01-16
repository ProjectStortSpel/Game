/*
Author: Anders, Christian
*/
#ifndef GRAPHICDEVICE_H
#define GRAPHICDEVICE_H

#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"
#include "Camera.h"
#include "SkyBox.h"

namespace Renderer
{
#define RENDER_FORWARD  1
#define RENDER_VIEWSPACE  2
#define RENDER_INTERFACE  3

#define TEXTURE_DIFFUSE		0
#define TEXTURE_NORMAL		1
#define TEXTURE_SPECULAR	2


	struct Model
	{
		bool operator== (const Model &m) { return Compare(m); }
		bool operator!= (const Model &m) { return !Compare(m); }

		Model(){}
		Model(Buffer* buffer, GLuint tex, GLuint nor, GLuint spe, int id, bool active, mat4* model)
		{
			bufferPtr = buffer;
			texID = tex;
			norID = nor;
			speID = spe;
			
			this->id = id;
			this->active = active;
			this->modelMatrix = model;
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
		
		int id;
		bool active;
		mat4* modelMatrix;
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
		bool PreLoadModel(std::string _dir, std::string _file, int _renderType = RENDER_FORWARD);
		int LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_FORWARD);
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);
		bool ChangeModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE);
		bool ChangeModelNormalMap(int _id, std::string _fileDir);
		bool ChangeModelSpecularMap(int _id, std::string _fileDir);

		void SetDebugTexFlag(int _flag) {  }
		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);

		int GetVRamUsage(){ return 0; }
		
		void Clear();

	private:
		bool InitSDLWindow();
		bool InitShaders();
		bool InitBuffers();
		bool InitSkybox();

		Camera* m_camera;

		SDL_Window*		m_window;
		SDL_GLContext	m_glContext;

		// dt and fps
		float m_dt;
		int m_fps;

		int m_vramUsage; //in bytes

		// Window size
		int	m_clientWidth, m_clientHeight;

		// Shaders
		Shader m_skyBoxShader;
		Shader m_forwardShader, m_viewspaceShader;

		// Skybox
		SkyBox *m_skybox;

		// Modelloader
		int m_modelIDcounter;
		std::vector<Model> m_modelsForward, m_modelsViewspace;

		// Meshs
		std::map<const std::string, Buffer*> m_meshs;
		Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg);
		// Textures
		std::map<const std::string, GLuint> m_textures;
		GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);

	};
}



#endif