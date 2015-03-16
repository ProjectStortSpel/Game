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
#include "Particles/ParticleEffect.h"
#include "Particles/Fire.h"
#include "Particles/Smoke.h"
#include "Particles/Waterfall.h"
#include "Particles/WaterSpawn.h"
#include "Particles/Explosion.h"
#include "Model.h"
#include "AModel.h"

namespace Renderer
{
#define RENDER_FORWARD  1
#define RENDER_VIEWSPACE  2
#define RENDER_INTERFACE  3
//#define RENDER_ANIMATED  4
#define RENDER_RIVERWATER 5
#define RENDER_RIVERWATER_CORNER 6

#define TEXTURE_DIFFUSE		0
#define TEXTURE_NORMAL		1
#define TEXTURE_SPECULAR	2


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
		bool hideInderface;

		GraphicDevice();
		GraphicDevice(Camera _camera);
		virtual ~GraphicDevice();

		// BASIC
		virtual bool Init(){ return false; };
		void PollEvent(SDL_Event _event);
		virtual void Update(float _dt){};
		virtual void Render(){};

		// WINDOW
		//SDL_Window*	GetSDL_Window(){ return m_window; } // IN PC GD
		//SDL_GLContext GetSDL_GLContext(){ return m_glContext; } // IN PC GD
		void GetWindowSize(int &x, int &y);
		void GetFramebufferSize(int &x, int &y);
		//void GetWindowPos(int &x, int &y); // IN PC GD
		void ResizeWindow(int _width, int _height);
		virtual void SetTitle(std::string _title){};

		// CAMERA
		Camera *GetCamera(){ return m_camera; }

		// MODEL
		virtual bool PreLoadModel(std::vector<std::string> _dirs, std::string _file, int _renderType = RENDER_FORWARD){ return false; }; // TODO: not in GD in PC
		virtual int LoadModel(std::vector<std::string> _dirs, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_FORWARD, float* _color = nullptr, bool _castShadow = true, bool _isStatic = false){ return 0; }; // TODO: no virtual
		int LoadModel(ModelToLoadFromSource* _modelToLoad);
		virtual bool RemoveModel(int _id){ return false; }; // TODO: no virtual
		virtual bool ActiveModel(int _id, bool _active){ return false; };  // TODO: no virtual

		// ANIMATIONS
		glm::mat4 GetJointMatrix(int _modelId, int _jointId); // IN PC GD
		bool SetAnimation(int _modelId, int _animId, float _frameTime); // IN PC GD

		// TEXTURES
		virtual bool ChangeModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE){ m_modelTextures.push_back({ _id, _fileDir, _textureType }); return false; };
		virtual bool ChangeModelNormalMap(int _id, std::string _fileDir){ m_modelTextures.push_back({ _id, _fileDir, TEXTURE_NORMAL }); return false; };
		virtual bool ChangeModelSpecularMap(int _id, std::string _fileDir){ m_modelTextures.push_back({ _id, _fileDir, TEXTURE_SPECULAR }); return false; };

		// LIGHTNING
		virtual void BufferPointlights(int _nrOfLights, float **_lightPointers){};
		virtual void BufferDirectionalLight(float *_lightPointer){};

		// TEXT RENDERING
		int AddFont(const std::string& filepath, int size);
		float CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size = glm::ivec2(-1, -1));
		void CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size = glm::ivec2(-1, -1));
		
		// PARTICLES
		void AddParticleEffect(std::string _name, const vec3 _pos, const vec3 _vel, int _nParticles, float _lifeTime, vec3 _scale, float _spriteSize, std::string _texture, vec3 _color, int &_id);
		void RemoveParticleEffect(int _id);
		void SetParticleAcceleration(int _id, float x, float y, float z);

		// SIMPLETEXT
		virtual bool RenderSimpleText(std::string _text, int x, int y){ return false; }
		virtual void SetSimpleTextColor(float _r, float _g, float _b, float _a){};
		virtual void SetDisco(){};
		virtual void ToggleSimpleText(){};
		virtual void ToggleSimpleText(bool _on){};

		// �VRIGT
		void SetDebugTexFlag(int _flag) { return; }
		void Clear(); // virtual in PC
		int GetVRamUsage(){ return -1; }

	protected:
		SDL_GLContext	m_glContext;
		bool			m_SDLinitialized;
		float			m_dt, m_elapsedTime;
		int				m_fps;

		// Window
		SDL_Window*		m_window;
		int	m_clientWidth, m_clientHeight;

		// Camera
		Camera* m_camera;

		// Models
		int m_modelIDcounter;
		bool m_useAnimations;
		//std::vector<RenderList> m_renderLists;
		std::vector<Model> m_modelsForward, m_modelsViewspace, m_modelsInterface, m_modelsWater, m_modelsWaterCorners;
		std::vector<AModel> m_modelsAnimated;
		std::map<int, ModelToLoad*> m_modelsToLoad;
		std::map<int, ModelToLoadFromSource*> m_modelsToLoadFromSource;
		std::map<int, std::vector<ModelToLoad*>> m_staticModelsToLoad;

		// Data
		std::vector<ModelTexture> m_modelTextures;
		std::map<const std::string, GLuint> m_textures;
		std::map<const std::string, Buffer*> m_meshs;
		std::vector<std::pair<std::string, SDL_Surface*>> m_surfaces;

		// Skybox
		SkyBox *m_skybox, *m_skyboxClouds;

		// Particles
		int m_particleID;
		std::map<int, ParticleEffect*> m_particleEffects;
		std::vector<ParticleSystemToLoad> m_particleSystemsToLoad;
		std::vector<int> m_particlesIdToRemove;

		// Shaders
		Shader m_fullscreen;
		Shader m_skyBoxShader, m_skyboxCloudsShader;
		Shader m_forwardShader;
		Shader m_viewspaceShader;
		Shader m_interfaceShader;
		Shader m_animationShader;
		//Shader m_shadowShaderForward;
		Shader m_riverShader, m_riverCornerShader;
		std::map<std::string, Shader*> m_particleShaders;

		// Lights
		vec3 m_dirLightDirection;
		float m_lightDefaults[10];
		float* m_directionalLightPtr;
		float** m_pointlightsPtr;
		int m_nrOfLightsToBuffer;


		// BASIC
		//virtual void InitRenderLists() { return; }
		void InitStandardBuffers();
		void InitStandardShaders();
		//bool InitSDLWindow(int _width = 1280, int _height = 720);
		void InitFBO();
		virtual bool InitLightBuffers() = 0;

		// Models
		virtual void BufferModels();	// TODO: no virtual
		virtual void BufferModel(int _modelId, ModelToLoad* _modelToLoad);	// TODO: no virtual
		void BufferModel(int _modelId, ModelToLoadFromSource* _modelToLoad);
		void BufferAModel(int _modelId, ModelToLoad* _modelToLoad);

		// Data
		virtual Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg, bool animated = false) = 0; // TODO: no virtual
		virtual Buffer* AddMesh(ModelToLoadFromSource* _modelToLoad, Shader *_shaderProg) = 0; // TODO: no virtual
		void SortModelsBasedOnDepth(std::vector<Model>* models);
		void BufferStaticModel(std::pair<int, std::vector<ModelToLoad*>> _staticModel);

		// Skybox
		bool InitSkybox();
		
		// PARTICLES
		void BufferParticleSystems();

		// TEXTURES	
		GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);
		void BufferModelTextures();
		virtual bool BufferModelTexture(int _id, std::string _fileDir, int _textureType);
		void BufferSurfaces();

		// Lights
		virtual void BufferLightsToGPU() = 0;
		void BufferLightsToGPU_GD();
		
		// The Framebuffer
		GLuint m_FBO;
		int m_framebufferWidth, m_framebufferHeight;
		// Framebuffer textures
		GLuint m_depthBuf, m_outputImage;

		GLuint m_fullscreenQuadBuffer;
		void CreateFullscreenQuad();

	};
}

#endif