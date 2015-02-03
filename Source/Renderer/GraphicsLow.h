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

	//	void PollEvent(SDL_Event _event);
		void Update(float _dt);
		void Render();

	//	void ResizeWindow(int _width, int _height);
		//void SetTitle(std::string _title);

		// SIMPLETEXT FROM GAME
		bool RenderSimpleText(std::string _text, int x, int y){ return false; }
		void SetSimpleTextColor(float _r, float _g, float _b, float _a){ return; }
		void SetDisco(){ return; }
		void ToggleSimpleText(){ return; }
		void ToggleSimpleText(bool _on){ return; }

	//	Camera *GetCamera(){ return m_camera; }
	//	void GetWindowSize(int &x, int &y){ x = m_clientWidth; y = m_clientHeight; }

		// MODELLOADER
		bool PreLoadModel(std::string _dir, std::string _file, int _renderType = RENDER_DEFERRED);
		int LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_DEFERRED);
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);
		bool ChangeModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE);
		bool ChangeModelNormalMap(int _id, std::string _fileDir);
		bool ChangeModelSpecularMap(int _id, std::string _fileDir);

	//	void SetDebugTexFlag(int _flag) { m_debugTexFlag = _flag; }
		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);
		
		void Clear();

	//	int GetVRamUsage(){ return m_vramUsage; }
		
	//	int AddFont(const std::string& filepath, int size);
	//	float CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size = glm::ivec2(-1, -1));
		//void CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size = glm::ivec2(-1, -1));
		
	private:
		bool InitSDLWindow();
		bool InitGLEW();
		bool InitDeferred();
		bool InitForward();
		bool InitShaders();
		bool InitBuffers();
		bool InitLightBuffers();
		//bool InitSkybox();
		bool InitRandomVector();

		void CreateShadowMap();

		//SDL_Window*		m_window;
		//SDL_GLContext	m_glContext;

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
		//GLuint m_skyBox;
		GLuint m_outputImage;

		// Frame buffer object
		GLuint m_forwardFBO;

		// Shaders
		//Shader m_fullScreenShader;
		//Shader m_skyBoxShader;
		Shader m_forwardShader;
		Shader m_viewspaceShader;
		Shader m_interfaceShader;

		Shader m_shadowShaderForward;

		//// Skybox
		//SkyBox *m_skybox;

		// Modelloader
		int m_modelIDcounter;
		std::vector<Model> m_modelsForward, m_modelsViewspace, m_modelsInterface;

		vec3 m_dirLightDirection;

		int m_nrOfLights; // lol

		// Meshs
		std::map<const std::string, Buffer*> m_meshs;
		Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg);
		// Textures
		//std::map<const std::string, GLuint> m_textures;
		//GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);

		// Random Vertors
		GLuint m_randomVectors;
		
		//TextRenderer m_sdlTextRenderer;
	};
}



#endif