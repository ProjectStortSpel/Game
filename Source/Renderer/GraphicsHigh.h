/*
Author: Anders, Christian
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
		GraphicsHigh(Camera _camera, int x, int y);
		~GraphicsHigh();

		bool Init();

		void Update(float _dt);
		void Render();

		// SIMPLETEXT FROM GAME
		bool RenderSimpleText(std::string _text, int x, int y);
		void SetSimpleTextColor(float _r, float _g, float _b, float _a);
		void SetDisco();
		void ToggleSimpleText();
		void ToggleSimpleText(bool _on);

		// MODELLOADER
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);

		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);
		
		void Clear();
		
	private:
		void PrintModelInfo(int setting);

		bool InitGLEW();
		bool InitDeferred();
		bool InitForward();
		bool InitShaders();
		void InitRenderLists();
		bool InitBuffers();
		bool InitTextRenderer();
		bool InitLightBuffers();
		bool InitRandomVector();

		void CreateGBufTex(GLenum texUnit, GLenum format, GLuint &texid);
		void CreateDepthTex(GLuint &texid);

		void BufferLightsToGPU();
		void CreateShadowMap();

		//holds the reset values for lights
		float m_lightDefaults[19];

		//Le shadowmap
		void WriteShadowMapDepth();

		// Timer for shader run time
		std::vector<GLTimerValue> m_glTimerValues;

		// Image buffers
		GLuint m_outputImage;
		GLuint m_debuggText;
		GLuint m_depthBuf, m_normTex, m_colorTex;

		// Frame buffer object
		GLuint m_deferredFBO, m_forwardFBO;

		// Shaders
		Shader m_fullScreenShader;
		Shader m_deferredShader1, m_compDeferredPass2Shader;
		Shader m_shadowShaderDeferred;
		Shader m_shadowShaderAnim;

		// SimpleText
		bool m_renderSimpleText;
		SimpleText m_textRenderer;

		// Modelloader
		int m_modelIDcounter;
		std::vector<Model> m_modelsDeferred;

		// Pointlights buffer
		GLuint m_pointlightBuffer, m_dirLightBuffer, m_animationBuffer;
		vec3 m_dirLightDirection;

		// DEBUG variables ----
		int m_nrOfLights; // lol

		// Random Vertors
		GLuint m_randomVectors;

		int mark;
		float timer;
        
        bool m_FBOsCreated;
	};
}



#endif