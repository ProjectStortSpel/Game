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
		bool PreLoadModel(std::string _dir, std::string _file, int _renderType = RENDER_DEFERRED);
		int LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_DEFERRED);
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);

		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);
		
		void Clear();
		
	private:
		bool InitSDLWindow();
		bool InitGLEW();
		bool InitDeferred();
		bool InitForward();
		bool InitShaders();
		bool InitBuffers();
		bool InitTextRenderer();
		bool InitLightBuffers();
		bool InitRandomVector();

		void CreateGBufTex(GLenum texUnit, GLenum format, GLuint &texid);
		void CreateDepthTex(GLuint &texid);

		void BufferLightsToGPU();
		void CreateShadowMap();

		void BufferModels();
		void BufferModel(int _modelId, ModelToLoad* _modelToLoad);

		//holds the reset values for lights
		float m_lightDefaults[19];

		//Le shadowmap
		ShadowMap *m_shadowMap;
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
		Shader m_forwardShader;
		Shader m_viewspaceShader;
		Shader m_interfaceShader;

		Shader m_shadowShaderDeferred, m_shadowShaderForward;

		// SimpleText
		bool m_renderSimpleText;
		SimpleText m_textRenderer;

		// Modelloader
		int m_modelIDcounter;
		std::vector<Model> m_modelsDeferred, m_modelsForward, m_modelsViewspace, m_modelsInterface;

		// Pointlights buffer
		GLuint m_pointlightBuffer, m_dirLightBuffer;
		vec3 m_dirLightDirection;

		// DEBUG variables ----
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