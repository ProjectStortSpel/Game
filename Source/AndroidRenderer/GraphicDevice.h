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
#include "ParticleEffect.h"
#include "Fire.h"
#include "Smoke.h"

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
		bool debugModelInfo;

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

		// ANIMATIONS
		bool SetAnimation(int _modelId, int _animId);

		// MODELLOADER
		virtual bool PreLoadModel(std::vector<std::string> _dirs, std::string _file, int _renderType = RENDER_FORWARD){ return false; };
		virtual int LoadModel(std::vector<std::string> _dirs, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_FORWARD, float* _color = nullptr){ return 0; };
		int LoadModel(ModelToLoadFromSource* _modelToLoad);
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

		void AddParticleEffect(std::string _name, const vec3 _pos, int _nParticles, float _lifeTime, float _scale, float _spriteSize, std::string _texture, vec3 _color, int &_id);
		void RemoveParticleEffect(int _id);
		void SetParticleAcceleration(int _id, float x, float y, float z);

	protected:
		bool InitSkybox();
		
		virtual bool InitLightBuffers() = 0;

		virtual void BufferLightsToGPU() = 0;

		void InitFBO();
		void InitStandardBuffers();
		void InitStandardShaders();

		Camera* m_camera;

		SDL_Window*		m_window;
		SDL_GLContext	m_glContext;

		// dt and fps
		float m_dt;
		int m_fps;

		// For the river animation
		float m_elapsedTime;

		bool m_SDLinitialized;

		// Window size
		int	m_clientWidth, m_clientHeight;

		// Light info
		vec3 m_dirLightDirection;
		float m_lightDefaults[10];
		float* m_directionalLightPtr;
		float** m_pointlightsPtr;
		int m_nrOfLightsToBuffer;

		// Particles stuff
		std::map<int, ParticleEffect*> m_particleEffects;
		// For adding and removing on main thread
		std::vector<ParticleSystemToLoad> m_particleSystemsToLoad;
		std::vector<int> m_particlesIdToRemove;
		int m_particleID;

		void BufferParticleSystems();

		// The Framebuffer
		GLuint m_FBO;
		int m_framebufferWidth, m_framebufferHeight;
		// Framebuffer textures
		GLuint m_depthBuf, m_outputImage;

		// Shaders
		Shader m_skyBoxShader;
		Shader m_forwardShader, m_viewspaceShader, m_interfaceShader;
		Shader m_riverShader, m_riverCornerShader;
		Shader m_fullscreen;
		std::map<std::string, Shader> m_particleShaders;

		// Skybox
		SkyBox *m_skybox;

		// Modelloader
		int m_modelIDcounter;
		std::vector<Model> m_modelsForward, m_modelsViewspace, m_modelsInterface, m_modelsWater, m_modelsWaterCorners;

		// Meshs
		std::map<const std::string, Buffer*> m_meshs;
		virtual Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg) = 0;
		virtual Buffer* AddMesh(ModelToLoadFromSource* _modelToLoad, Shader *_shaderProg) = 0;
		// Textures
		std::map<const std::string, GLuint> m_textures;
		GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);
		
		
		std::map<int, ModelToLoad*> m_modelsToLoad;
		std::map<int, ModelToLoadFromSource*> m_modelsToLoadFromSource;
		virtual void BufferModels();
		virtual void BufferModel(int _modelId, ModelToLoad* _modelToLoad);
		void BufferModel(int _modelId, ModelToLoadFromSource* _modelToLoad);
		
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

		void BufferLightsToGPU_GD();
	};
}

#endif