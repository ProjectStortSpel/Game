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
#define RENDER_RIVERWATER 5
#define RENDER_RIVERWATER_CORNER 6

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
		bool CastShadow;
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
		bool CastShadow;
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
		vec3 Vel;
		int NrOfParticles;
		float LifeTime;
		vec3 Scale;
		float SpriteSize;
		std::string TextureName;
		vec3 Color;
		int Id;
	};

	class DECLSPEC GraphicDevice
	{
	public:
		bool debugModelInfo;

		GraphicDevice();
		GraphicDevice(Camera _camera, int x, int y);
		virtual ~GraphicDevice();

		// BASIC
		virtual bool Init(){ return false; };// = 0;
		void PollEvent(SDL_Event _event);
		virtual void Update(float _dt){};// = 0;
		virtual void Render(){};// = 0;

		// WINDOW
		SDL_Window*	GetSDL_Window(){ return m_window; }
		SDL_GLContext GetSDL_GLContext(){ return m_glContext; }
		void GetWindowSize(int &x, int &y){ x = m_clientWidth; y = m_clientHeight; }
		void GetWindowPos(int &x, int &y);
		void ResizeWindow(int _width, int _height);
		virtual void SetTitle(std::string _title){};// = 0;

		// CAMERA
		Camera* GetCamera(){ return m_camera; }

		// MODEL
		int LoadModel(std::vector<std::string> _dirs, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_DEFERRED, float* _color = nullptr, bool _castShadow = true, bool _isStatic = false);
		int LoadModel(ModelToLoadFromSource* _modelToLoad);
		bool RemoveModel(int _id);// = 0;
		bool ActiveModel(int _id, bool _active);// = 0;

		// ANIMATIONS
		glm::mat4 GetJointMatrix(int _modelId, int _jointId);
		bool SetAnimation(int _modelId, int _animId, float _frameTime);

		// TEXTURES
		virtual bool ChangeModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE){ m_modelTextures.push_back({ _id, _fileDir, _textureType }); return false; };// = 0;
		virtual bool ChangeModelNormalMap(int _id, std::string _fileDir){ m_modelTextures.push_back({ _id, _fileDir, TEXTURE_NORMAL }); return false; };// = 0;
		virtual bool ChangeModelSpecularMap(int _id, std::string _fileDir){ m_modelTextures.push_back({ _id, _fileDir, TEXTURE_SPECULAR }); return false; };// = 0;

		// LIGHTNING
		virtual void BufferPointlights(int _nrOfLights, float **_lightPointers){};// = 0;
		virtual void BufferDirectionalLight(float *_lightPointer){};// = 0;
		
		// TEXT RENDERING
		int AddFont(const std::string& filepath, int size);
		float CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size = glm::ivec2(-1, -1));
		void CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size = glm::ivec2(-1, -1));

		// PARTICLES
		void AddParticleEffect(std::string _name, const vec3 _pos, const vec3 _vel, int _nParticles, float _lifeTime, vec3 _scale, float _spriteSize, std::string _texture, vec3 _color, int &_id);
		void RemoveParticleEffect(int _id);
		void SetParticleAcceleration(int _id, float x, float y, float z);
		
		// SIMPLETEXT
		virtual bool RenderSimpleText(std::string _text, int x, int y){ return false; };// = 0;
		virtual void SetSimpleTextColor(float _r, float _g, float _b, float _a){};// = 0;
		virtual void SetDisco(){};// = 0;
		virtual void ToggleSimpleText(){};// = 0;
		virtual void ToggleSimpleText(bool _on){};// = 0

		// ÖVRIGT
		void SetDebugTexFlag(int _flag) { m_debugTexFlag = _flag; }
		virtual void Clear(){};// = 0;
		int GetVRamUsage(){ return m_vramUsage; }

	protected:
		SDL_GLContext	m_glContext;
		bool			m_SDLinitialized;
		int				m_vramUsage; //in bytes
		float			m_dt, m_elapsedTime;
		int				m_fps;
		int				m_debugTexFlag;

		// Window
		SDL_Window*		m_window;
		const char*		m_windowCaption;
		int	m_clientWidth, m_clientHeight;
		int m_windowPosX, m_windowPosY;

		// Camera
		Camera*			m_camera;

		// Models
		int m_modelIDcounter;
		bool m_useAnimations;
		std::vector<RenderList> m_renderLists;
		std::vector<Model> m_modelsForward, m_modelsViewspace, m_modelsInterface, m_modelsWater, m_modelsWaterCorners;
		std::vector<Model*> m_shadowModelsForward, m_shadowModelsDeferred;
		std::vector<AModel> m_modelsAnimated;
		std::map<int, ModelToLoad*>					m_modelsToLoad;
		std::map<int, ModelToLoadFromSource*>		m_modelsToLoadFromSource;
		std::map<int, std::vector<ModelToLoad*>>	m_staticModelsToLoad;

		// Data
		std::vector<ModelTexture> m_modelTextures;
		std::map<const std::string, GLuint> m_textures;
		std::map<const std::string, Buffer*> m_meshs;
		std::vector<std::pair<std::string, SDL_Surface*>> m_surfaces;

		// Skybox
		SkyBox *m_skybox;

		// Particles
		int m_particleID;
		std::map<int, ParticleSystem*> m_particleSystems;
		std::vector<ParticleSystemToLoad> m_particleSystemsToLoad;
		std::vector<int> m_particlesIdToRemove;

		// Shaders
		Shader m_animationShader;
		Shader m_skyBoxShader;
		Shader m_forwardShader;
		Shader m_viewspaceShader;
		Shader m_interfaceShader;
		Shader m_shadowShaderForward, m_shadowShaderForwardAnim;
		Shader m_riverShader, m_riverCornerShader;
		std::map<std::string, Shader> m_particleShaders;

		// Lights
		float**	m_pointerToPointlights;
		int		m_numberOfPointlights;
		float*	m_pointerToDirectionalLights;
		int		m_numberOfDirectionalLights;

		// BASIC
		virtual void InitRenderLists() { return; }
		void InitStandardShaders();
		void InitStandardBuffers();
		bool InitSDLWindow(int _width = 1280, int _height = 720);

		// Models
		void BufferModels();
		void BufferModel(int _modelId, ModelToLoad* _modelToLoad);
		void BufferModel(int _modelId, ModelToLoadFromSource* _modelToLoad);
		void BufferAModel(int _modelId, ModelToLoad* _modelToLoad);

		// Data
		Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg, bool animated);
		Buffer* AddMesh(ModelToLoadFromSource* _modelToLoad, Shader *_shaderProg);
		void SortModelsBasedOnDepth(std::vector<Model>* models);
		void BufferStaticModel(std::pair<int, std::vector<ModelToLoad*>> _staticModel);

		// Skybox
		bool InitSkybox();

		// PARTICLES
		void BufferParticleSystems();

		// TEXTURES		
		GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);
		void BufferModelTextures();
		bool BufferModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE);
		void UpdateTextureIndex(GLuint newTexture, GLuint oldTexture);
		void BufferSurfaces();
	};
}



#endif