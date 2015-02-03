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
		~GraphicsLow();

		bool Init();

		void Update(float _dt);
		void Render();

		// SIMPLETEXT FROM GAME
		bool RenderSimpleText(std::string _text, int x, int y){ return false; }
		void SetSimpleTextColor(float _r, float _g, float _b, float _a){ return; }
		void SetDisco(){ return; }
		void ToggleSimpleText(){ return; }
		void ToggleSimpleText(bool _on){ return; }

		// MODELLOADER
		bool PreLoadModel(std::string _dir, std::string _file, int _renderType = RENDER_DEFERRED);
		int LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_DEFERRED);
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);

	//	void SetDebugTexFlag(int _flag) { m_debugTexFlag = _flag; }
		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);
		
		void Clear();
		
	private:
		bool InitSDLWindow();
		bool InitGLEW();
		bool InitDeferred();
		bool InitShaders();
		bool InitBuffers();
		bool InitLightBuffers();

		
		void BufferLightsToGPU();
		void CreateShadowMap();

		void BufferModels();
		void BufferModel(int _modelId, ModelToLoad* _modelToLoad);

		// dt and fps
		float m_dt;
		int m_fps;

		//holds the reset values for lights
		float m_lightDefaults[19];

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

		int m_nrOfLights; // lol

		// Meshs
		std::map<const std::string, Buffer*> m_meshs;
		Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg);

		// Random Vertors
		GLuint m_randomVectors;
		
		bool BufferModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE);
	};
}



#endif