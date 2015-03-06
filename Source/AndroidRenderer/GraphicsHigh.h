/*
--ANDROID VERSION--
Author: Christian
*/
#ifndef GRAPHICSHIGH_H
#define GRAPHICSHIGH_H

#include "GraphicDevice.h"

namespace Renderer
{
	class DECLSPEC GraphicsHigh : public GraphicDevice
	{
	public:
		GraphicsHigh();
		GraphicsHigh(Camera _camera);
		~GraphicsHigh();

		bool Init();

		void Update(float _dt);
		void Render();

		// MODELLOADER
		bool PreLoadModel(std::vector<std::string> _dirs, std::string _file, int _renderType = RENDER_FORWARD);
		int LoadModel(std::vector<std::string> _dirs, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_FORWARD, float* _color = nullptr, bool _castShadow = true);
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);

		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);
		
		void Clear();

	private:
		bool InitSDLWindow();
		bool InitShaders();
		bool InitBuffers();
		bool InitLightBuffers();

		void BufferLightsToGPU();

		// Shadow 
		ShadowMap *m_shadowMap;

		// Shaders
		Shader m_shadowShader;

		void CreateShadowMap();
		void WriteShadowMapDepth();

		// Meshs
		Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg);
		Buffer* AddMesh(ModelToLoadFromSource* _modelToLoad, Shader *_shaderProg);
	};
}



#endif