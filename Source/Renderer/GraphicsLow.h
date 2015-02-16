/*
Author: Christian
*/
#ifndef GRAPHICSLOW_H
#define GRAPHICSLOW_H

#include "GraphicDevice.h"

namespace Renderer
{

	class DECLSPEC GraphicsLow : public GraphicDevice
	{
	public:
		GraphicsLow();
		GraphicsLow(Camera _camera, int x, int y);
		~GraphicsLow();

		bool Init();

		void Update(float _dt);
		void Render();

		// MODELLOADER
		bool PreLoadModel(std::string _dir, std::string _file, int _renderType = RENDER_DEFERRED);
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);

		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);
		
		void Clear();
		
	protected:
		void UpdateTextureIndex(GLuint newTexture, GLuint oldTexture);
		
	private:
		bool InitGLEW();
		bool InitDeferred();
		bool InitShaders();
		void InitModelLists();
		bool InitBuffers();
		bool InitLightBuffers();

		
		void BufferLightsToGPU();
		void CreateShadowMap();

		void BufferModels();
		void BufferModel(int _modelId, ModelToLoad* _modelToLoad);

		//holds the reset values for lights
		float m_lightDefaults[10];

		//Le shadowmap
		ShadowMap *m_shadowMap;
		void WriteShadowMapDepth();

		// Timer for shader run time
		std::vector<GLTimerValue> m_glTimerValues;

		// Image buffers
		GLuint m_outputImage;

		// Frame buffer object
		GLuint m_forwardFBO;

		// Shaders
		Shader m_forwardShader;
		Shader m_viewspaceShader;
		Shader m_interfaceShader;

		Shader m_shadowShaderForward;

		// Modelloader
		int m_modelIDcounter;
		std::vector<Model> m_modelsForward, m_modelsViewspace, m_modelsInterface;

		vec3 m_dirLightDirection;

		int m_nrOfLightsToBuffer; // lol

		// Meshs
		std::map<const std::string, Buffer*> m_meshs;
		Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg);

		// Random Vertors
		GLuint m_randomVectors;
		
		bool BufferModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE);
	};
}



#endif