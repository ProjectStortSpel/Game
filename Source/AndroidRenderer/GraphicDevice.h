/*
--ANDROID VERSION--
Author: Christian
*/
#ifndef GRAPHICDEVICE_H
#define GRAPHICDEVICE_H

#include "stdafx.h"
#include "Shader.h"
#include "Buffer.h"
#include "Camera.h"
#include "SkyBox.h"
#include "ShadowMap.h"
#include "TextRenderer.h"
#include "SkyBox.h"
#include "ModelLoader.h"

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
		Model(Buffer* buffer, GLuint tex, GLuint nor, GLuint spe, int id, bool active, mat4* model, float* col)
		{
			bufferPtr = buffer;
			texID = tex;
			norID = nor;
			speID = spe;
			
			this->id = id;
			this->active = active;
			this->modelMatrix = model;
			this->color = col;
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
		float* color;
	};

	struct ModelToLoad
	{
		std::string Dir;
		std::string File;
		glm::mat4* MatrixPtr;
		int RenderType;
		float* Color;
	};

	class DECLSPEC GraphicDevice
	{
	public:
		GraphicDevice();
		GraphicDevice(Camera _camera);
		virtual ~GraphicDevice();

		virtual bool Init(){ return false; };

		void PollEvent(SDL_Event _event);
		virtual void Update(float _dt){};
		virtual void Render(){};

		void ResizeWindow(int _width, int _height);
		virtual void SetTitle(std::string _title){};

		// SIMPLETEXT FROM GAME
		virtual bool RenderSimpleText(std::string _text, int x, int y){ return false; }
		virtual void SetSimpleTextColor(float _r, float _g, float _b, float _a){};
		virtual void SetDisco(){};
		virtual void ToggleSimpleText(){};
		virtual void ToggleSimpleText(bool _on){};

		Camera *GetCamera(){ return m_camera; }
		void GetWindowSize(int &x, int &y){ x = m_clientWidth; y = m_clientHeight; }

		// MODELLOADER
		virtual bool PreLoadModel(std::string _dir, std::string _file, int _renderType = RENDER_FORWARD){ return false; };
		virtual int LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_FORWARD, float* _color = nullptr){ return 0; };
		virtual bool RemoveModel(int _id){ return false; };
		virtual bool ActiveModel(int _id, bool _active){ return false; };
		virtual bool ChangeModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE){ m_modelTextures.push_back({ _id, _fileDir, _textureType }); return false; };
		virtual bool ChangeModelNormalMap(int _id, std::string _fileDir){ m_modelTextures.push_back({ _id, _fileDir, TEXTURE_NORMAL }); return false; };
		virtual bool ChangeModelSpecularMap(int _id, std::string _fileDir){ m_modelTextures.push_back({ _id, _fileDir, TEXTURE_SPECULAR }); return false; };

		void SetDebugTexFlag(int _flag) { return; }
		virtual void BufferPointlights(int _nrOfLights, float **_lightPointers){};
		virtual void BufferDirectionalLight(float *_lightPointer){};

		int GetVRamUsage(){ return -1; }
		
		virtual void Clear(){};
		
		int AddFont(const std::string& filepath, int size);
		float CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size = glm::ivec2(-1, -1));
		void CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size = glm::ivec2(-1, -1));

	protected:
		bool InitSkybox();
		
		virtual bool InitLightBuffers() = 0;

		virtual void BufferLightsToGPU() = 0;

		Camera* m_camera;

		SDL_Window*		m_window;
		SDL_GLContext	m_glContext;

		// dt and fps
		float m_dt;
		int m_fps;

		bool m_SDLinitialized;

		// Window size
		int	m_clientWidth, m_clientHeight;

		// Light info
		vec3 m_dirLightDirection;
		float m_lightDefaults[10];
		float* m_directionalLightPtr;
		float** m_pointlightsPtr;
		int m_nrOfLightsToBuffer;

		// Shaders
		Shader m_skyBoxShader;
		Shader m_forwardShader, m_viewspaceShader, m_interfaceShader;

		// Skybox
		SkyBox *m_skybox;

		// Modelloader
		int m_modelIDcounter;
		std::vector<Model> m_modelsForward, m_modelsViewspace, m_modelsInterface;

		// Meshs
		std::map<const std::string, Buffer*> m_meshs;
		virtual Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg) = 0;
		// Textures
		std::map<const std::string, GLuint> m_textures;
		GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);
		
		
		std::map<int, ModelToLoad*> m_modelsToLoad;
		virtual void BufferModels();
		virtual void BufferModel(int _modelId, ModelToLoad* _modelToLoad);
		
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
		virtual bool BufferModelTexture(int _id, std::string _fileDir, int _textureType);
		
		void SortModelsBasedOnDepth(std::vector<Model>* models);
	};
}

#endif