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
#include "TextRenderer.h"

namespace Renderer
{
#define RENDER_DEFERRED 0
#define RENDER_FORWARD  1
#define RENDER_VIEWSPACE  2
#define RENDER_INTERFACE  3

#define TEXTURE_DIFFUSE		0
#define TEXTURE_NORMAL		1
#define TEXTURE_SPECULAR	2

	struct Instance
	{
		int id;
		bool active;
		bool viewspace;
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

	struct ModelToLoad
	{
		std::string Dir;
		std::string File;
		glm::mat4* MatrixPtr;
		int RenderType;
	};

	class DECLSPEC GraphicDevice
	{
	public:
		GraphicDevice(){}
		virtual ~GraphicDevice();

		virtual bool Init(){ return false; };// = 0;

		void PollEvent(SDL_Event _event);
		virtual void Update(float _dt){};// = 0;
		virtual void Render(){};// = 0;

		void ResizeWindow(int _width, int _height);
		virtual void SetTitle(std::string _title){};// = 0;

		// SIMPLETEXT FROM GAME
		virtual bool RenderSimpleText(std::string _text, int x, int y){ return false; };// = 0;
		virtual void SetSimpleTextColor(float _r, float _g, float _b, float _a){};// = 0;
		virtual void SetDisco(){};// = 0;
		virtual void ToggleSimpleText(){};// = 0;
		virtual void ToggleSimpleText(bool _on){};// = 0;

		Camera *GetCamera(){ return m_camera; }
		void GetWindowSize(int &x, int &y){ x = m_clientWidth; y = m_clientHeight; }

		// MODELLOADER
		virtual bool PreLoadModel(std::string _dir, std::string _file, int _renderType = RENDER_DEFERRED){ return false; };// = 0;
		virtual int LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_DEFERRED){ return 0; };// = 0;
		virtual bool RemoveModel(int _id){ return false; };// = 0;
		virtual bool ActiveModel(int _id, bool _active){ return false; };// = 0;
		virtual bool ChangeModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE){ m_modelTextures.push_back({ _id, _fileDir, _textureType }); return false; };// = 0;
		virtual bool ChangeModelNormalMap(int _id, std::string _fileDir){ m_modelTextures.push_back({ _id, _fileDir, TEXTURE_NORMAL }); return false; };// = 0;
		virtual bool ChangeModelSpecularMap(int _id, std::string _fileDir){ m_modelTextures.push_back({ _id, _fileDir, TEXTURE_SPECULAR }); return false; };// = 0;

		void SetDebugTexFlag(int _flag) { m_debugTexFlag = _flag; }
		virtual void BufferPointlights(int _nrOfLights, float **_lightPointers){};// = 0;
		virtual void BufferDirectionalLight(float *_lightPointer){};// = 0;
		
		virtual void Clear(){};// = 0;

		int GetVRamUsage(){ return m_vramUsage; }
		
		int AddFont(const std::string& filepath, int size);
		float CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size = glm::ivec2(-1, -1));
		void CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size = glm::ivec2(-1, -1));
		
	protected:
		bool InitSkybox();
		virtual void BufferModels() { return; } // = 0;
		virtual void BufferModel(int _modelId, ModelToLoad* _modelToLoad) { return; } // = 0;

		SDL_Window*		m_window;
		SDL_GLContext	m_glContext;

		Camera* m_camera;
		int m_vramUsage; //in bytes

		//// Window size
		int	m_clientWidth, m_clientHeight;

		// Shaders
		Shader m_skyBoxShader;

		// Skybox
		SkyBox *m_skybox;

		//// DEBUG variables ----
		int m_debugTexFlag;
		std::map<int, ModelToLoad*> m_modelsToLoad;
		float**	m_pointerToPointlights;
		int		m_numberOfPointlights;
		float*	m_pointerToDirectionalLights;
		int		m_numberOfDirectionalLights;

		GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);
		std::map<const std::string, GLuint> m_textures;

		TextRenderer m_sdlTextRenderer;
		
		std::vector<std::pair<std::string, SDL_Surface*>> m_surfaces;
		void BufferSurfaces();
		
		struct ModelTexture
		{
			int id;
			std::string textureName;
			int textureType;
		};
		std::vector<ModelTexture> m_modelTextures;
		void BufferModelTextures();
		virtual bool BufferModelTexture(int _id, std::string _fileDir, int _textureType) = 0;
	};
}



#endif