/*
--ANDROID VERSION--
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
		GraphicsLow(Camera _camera);
		~GraphicsLow();

		bool Init();

		void Update(float _dt);
		void Render();

		// MODELLOADER
		bool PreLoadModel(std::string _dir, std::string _file, int _renderType = RENDER_FORWARD);
		int LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_FORWARD);
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);

		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);
		
		void Clear();

	private:
		bool InitSDLWindow();
		bool InitShaders();
		bool InitBuffers();

		void BufferLightsToGPU();

		// Shaders
		//Shader m_fullscreen;

		// Meshs
		Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg);
		
	};
}



#endif