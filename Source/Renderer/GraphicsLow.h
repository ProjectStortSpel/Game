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
		bool PreLoadModel(std::vector<std::string> _dirs, std::string _file, int _renderType = RENDER_DEFERRED);
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);

		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);
		
		void Clear();
		
	private:
		bool InitGLEW();
		bool InitDeferred();
		bool InitShaders();
		void InitRenderLists();
		bool InitBuffers();
		bool InitLightBuffers();
		
		void BufferLightsToGPU();
		void CreateShadowMap();

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

		// Modelloader
		int m_modelIDcounter;
		std::vector<Model> m_modelsForward, m_modelsViewspace, m_modelsInterface;

		vec3 m_dirLightDirection;

		int m_nrOfLightsToBuffer; // lol

		// Random Vertors
		GLuint m_randomVectors;

	};
}



#endif