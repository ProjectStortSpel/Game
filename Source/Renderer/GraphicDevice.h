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
#include "ParticleSystem.h"
#include "AModel.h"
#include "Model.h"

namespace Renderer
{
#define RENDER_DEFERRED 0
#define RENDER_FORWARD  1
#define RENDER_VIEWSPACE  2
#define RENDER_INTERFACE  3
#define RENDER_ANIMATED  4

#define RENDER_DEFERRED_SCATTER  8
#define RENDER_FORWARD_SCATTER  9

#define TEXTURE_DIFFUSE		0
#define TEXTURE_NORMAL		1
#define TEXTURE_SPECULAR	2

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

	struct RenderList
	{
		int RenderType;
		std::vector<Model>* ModelList;
		Shader* ShaderPtr;
		RenderList(int _renderType, std::vector<Model>* _modelList, Shader* _shaderPtr)
		{
			RenderType = _renderType;
			ModelList = _modelList;
			ShaderPtr = _shaderPtr;
		}
	};

	struct ModelToLoad
	{
		std::vector<std::string> Dirs;
		std::string File;
		glm::mat4* MatrixPtr;
		int RenderType;
		float* Color;
	};
	
	struct ModelToLoadFromSource
	{
		std::string key;
		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<float> tangents;
		std::vector<float> bitangents;
		std::vector<float> texCoords;
		std::string diffuseTextureFilepath;
		std::string normalTextureFilepath;
		std::string specularTextureFilepath;
		glm::mat4* MatrixPtr;
		int RenderType;
		float* Color;
	};

	struct ModelTexture
	{
		int id;
		std::string textureName;
		int textureType;
	};

	struct ParticleSystemToLoad
	{
		std::string Name;
		vec3 Pos;
		int NrOfParticles;
		float LifeTime;
		float Scale;
		float SpriteSize;
		std::string TextureName;
		vec3 Color;
		int Id;
	};

	class DECLSPEC GraphicDevice
	{
	public:
		GraphicDevice();
		GraphicDevice(Camera _camera, int x, int y);
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

		Camera* GetCamera(){ return m_camera; }

		SDL_Window*	GetSDL_Window(){ return m_window; }
		SDL_GLContext GetSDL_GLContext(){ return m_glContext; }

		void GetWindowSize(int &x, int &y){ x = m_clientWidth; y = m_clientHeight; }

		void GetWindowPos(int &x, int &y);

		// MODELLOADER
		int LoadModel(std::vector<std::string> _dirs, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_DEFERRED, float* _color = nullptr);
		int LoadModel(ModelToLoadFromSource* _modelToLoad);
		bool RemoveModel(int _id);// = 0;
		bool ActiveModel(int _id, bool _active);// = 0;
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

		void AddParticleEffect(std::string _name, const vec3 _pos, int _nParticles, float _lifeTime, float _scale, float _spriteSize, std::string _texture, vec3 _color, int &_id);
		void RemoveParticleEffect(int _id);
		void SetParticleAcceleration(int _id, float x, float y, float z);
		
	protected:
		virtual void InitRenderLists() { return; }
		bool InitSDLWindow(int _width = 1280, int _height = 720);
		bool InitSkybox();
		void BufferModels();
		void BufferModel(int _modelId, ModelToLoad* _modelToLoad);
		void BufferAModel(int _modelId, ModelToLoad* _modelToLoad);
		void BufferModel(int _modelId, ModelToLoadFromSource* _modelToLoad);

		std::vector<ModelTexture> m_modelTextures;
		void BufferModelTextures();
		bool BufferModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE);

		void UpdateTextureIndex(GLuint newTexture, GLuint oldTexture);
		// Meshs
		std::map<const std::string, Buffer*> m_meshs;
		Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg, bool animated);
		Buffer* AddMesh(ModelToLoadFromSource* _modelToLoad, Shader *_shaderProg);

		//modellists
		std::vector<RenderList> m_renderLists;
		std::vector<AModel> m_modelsAnimated;
		Shader m_animationShader;

		//MODEL LOADER
		int m_modelIDcounter;

		SDL_Window*		m_window;
		SDL_GLContext	m_glContext;

		Camera* m_camera;
		int m_vramUsage; //in bytes

		// dt and fps
		float m_dt;
		int m_fps;

		const char*	m_windowCaption;
		bool m_SDLinitialized;

		//// Window size
		int	m_clientWidth, m_clientHeight;
		int m_windowPosX, m_windowPosY;

		// Shaders
		Shader m_skyBoxShader;
		std::map<std::string, Shader> m_particleShaders;

		// Skybox
		SkyBox *m_skybox;

		// Particles
		std::map<int, ParticleSystem*> m_particleSystems;
		int m_particleID;
		std::vector<ParticleSystemToLoad> m_particleSystemsToLoad;
		std::vector<int> m_particlesIdToRemove;

		//// DEBUG variables ----
		int m_debugTexFlag;
		std::map<int, ModelToLoad*> m_modelsToLoad;
		std::map<int, ModelToLoadFromSource*> m_modelsToLoadFromSource;
		float**	m_pointerToPointlights;
		int		m_numberOfPointlights;
		float*	m_pointerToDirectionalLights;
		int		m_numberOfDirectionalLights;

		GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);
		std::map<const std::string, GLuint> m_textures;
		
		std::vector<std::pair<std::string, SDL_Surface*>> m_surfaces;
		void BufferSurfaces();

		void SortModelsBasedOnDepth(std::vector<Model>* models);
		
		void CreateParticleSystems();

		void BufferParticleSystems();
		
	};
}



#endif