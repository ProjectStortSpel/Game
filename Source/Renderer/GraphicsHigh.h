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

	//	void PollEvent(SDL_Event _event);
		void Update(float _dt);
		void Render();

	//	void ResizeWindow(int _width, int _height);
		//void SetTitle(std::string _title);

		// SIMPLETEXT FROM GAME
		bool RenderSimpleText(std::string _text, int x, int y);
		void SetSimpleTextColor(float _r, float _g, float _b, float _a);
		void SetDisco();
		void ToggleSimpleText();
		void ToggleSimpleText(bool _on);

	//	Camera *GetCamera(){ return m_camera; }
	//	void GetWindowSize(int &x, int &y){ x = m_clientWidth; y = m_clientHeight; }

		// MODELLOADER
		bool PreLoadModel(std::string _dir, std::string _file, int _renderType = RENDER_DEFERRED);
		int LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType = RENDER_DEFERRED);
		bool RemoveModel(int _id);
		bool ActiveModel(int _id, bool _active);

	//	void SetDebugTexFlag(int _flag) { m_debugTexFlag = _flag; }
		void BufferPointlights(int _nrOfLights, float **_lightPointers);
		void BufferDirectionalLight(float *_lightPointer);
		
		void Clear();

	//	int GetVRamUsage(){ return m_vramUsage; }
		
	//	int AddFont(const std::string& filepath, int size);
	//	float CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size = glm::ivec2(-1, -1));
	//	void CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size = glm::ivec2(-1, -1));
		
	private:
		bool InitSDLWindow();
		bool InitGLEW();
		bool InitDeferred();
		bool InitForward();
		bool InitShaders();
		bool InitBuffers();
		bool InitTextRenderer();
		bool InitLightBuffers();
	//	bool InitSkybox();
		bool InitRandomVector();

		void CreateGBufTex(GLenum texUnit, GLenum format, GLuint &texid);
		void CreateDepthTex(GLuint &texid);

		void BufferLightsToGPU();
		void CreateShadowMap();

		void BufferModels();
		void BufferModel(int _modelId, ModelToLoad* _modelToLoad);

		//Camera* m_camera;

	//	SDL_Window*		m_window;
	//	SDL_GLContext	m_glContext;

		// dt and fps
		float m_dt;
		int m_fps;

	//	int m_vramUsage; //in bytes

		//holds the reset values for lights
		float m_lightDefaults[19];

		//Le shadowmap
		ShadowMap *m_shadowMap;
		void WriteShadowMapDepth();

		// Timer for shader run time
		std::vector<GLTimerValue> m_glTimerValues;

		// Window size
		//int	m_clientWidth, m_clientHeight;

		// Image buffers
	//	GLuint m_skyBox;
		GLuint m_outputImage;
		GLuint m_debuggText;
		GLuint m_depthBuf, m_normTex, m_colorTex;

		// Frame buffer object
		GLuint m_deferredFBO, m_forwardFBO;

		// Shaders
		Shader m_fullScreenShader;
	//	Shader m_skyBoxShader;
		Shader m_deferredShader1, m_compDeferredPass2Shader;
		Shader m_forwardShader;
		Shader m_viewspaceShader;
		Shader m_interfaceShader;

		Shader m_shadowShaderDeferred, m_shadowShaderForward;

		// Skybox
	//	SkyBox *m_skybox;

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
		//int m_debugTexFlag; // 0=standard, 1=diffuse, 2=normal, 3=specular+shine, 4=glow
		int m_nrOfLights; // lol

		// Objects
		//std::map<const std::string, ObjectData> m_objects;
		//class ObjectData AddObject(std::string _file, std::string _dir);
		// Meshs
		std::map<const std::string, Buffer*> m_meshs;
		Buffer* AddMesh(std::string _fileDir, Shader *_shaderProg);
		// Textures
	//	std::map<const std::string, GLuint> m_textures;
	//	GLuint AddTexture(std::string _fileDir, GLenum _textureSlot);

		// Random Vertors
		GLuint m_randomVectors;
		
	//	TextRenderer m_sdlTextRenderer;
		
		bool BufferModelTexture(int _id, std::string _fileDir, int _textureType = TEXTURE_DIFFUSE);
	};
}



#endif