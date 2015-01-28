#include "GraphicDevice.h"

#define STB_IMAGE_IMPLEMENTATION
#include "TextureLoader.h"
#include "ModelLoader.h"
#include "ModelExporter.h"
#include "SkyBox.h"

using namespace Renderer;
using namespace glm;

GraphicDevice::GraphicDevice()
{
	m_modelIDcounter = 0;
}

GraphicDevice::~GraphicDevice()
{
	delete(m_camera);
	delete(m_skybox);
	// Delete buffers
	for (std::map<const std::string, Buffer*>::iterator it = m_meshs.begin(); it != m_meshs.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	
	for (std::map<const std::string, GLuint>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		glDeleteTextures(1, &it->second);
	}

	SDL_GL_DeleteContext(m_glContext);
	// Close and destroy the window
	SDL_DestroyWindow(m_window);
	// Clean up
	SDL_Quit();
}

bool GraphicDevice::Init()
{
	if (!InitSDLWindow()) { ERRORMSG("INIT SDL WINDOW FAILED\n"); return false; }

	m_camera = new Camera(m_clientWidth, m_clientHeight);

	if (!InitShaders()) { ERRORMSG("INIT SHADERS FAILED\n"); return false; }
	if (!InitBuffers()) { ERRORMSG("INIT BUFFERS FAILED\n"); return false; }
	if (!InitSkybox()) { ERRORMSG("INIT SKYBOX FAILED\n"); return false; }

	CreateShadowMap();
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.2f, 0.6f, 1.0f);

	return true;
}

void GraphicDevice::PollEvent(SDL_Event _event)
{
	switch (_event.type)
	{
	case SDL_WINDOWEVENT:
		switch (_event.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			int w, h;
			SDL_GetWindowSize(m_window, &w, &h);
			ResizeWindow(w,h);
			break;
		}
		break;

	default:
		break;
	}
}

void GraphicDevice::Update(float _dt)
{
	m_camera->Update(_dt);

  //SDL_Log("FPS: %f", 1.0f/_dt); 
}

void GraphicDevice::WriteShadowMapDepth()
{
    GLint oldFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
    
	//------- Write shadow maps depths ----------
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMap->GetShadowFBOHandle());
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glViewport(0, 0, m_shadowMap->GetResolution()-2, m_shadowMap->GetResolution()-2);

	m_shadowShader.UseProgram();
	//------FORWARD RENDERING--------------------------------------------
	glEnable(GL_BLEND);

	//glCullFace(GL_FRONT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(4.5, 18000.0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	mat4 shadowViewProj = (*m_shadowMap->GetProjectionMatrix()) * (*m_shadowMap->GetViewMatrix());

	//Forward models
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].active) // IS MODEL ACTIVE?
		{
			
			mat4 modelMatrix;
			if (m_modelsForward[i].modelMatrix == NULL)
			{
				modelMatrix = glm::translate(glm::vec3(1));
				SDL_Log("model: %d has no model matrix", i);
			}
			else
				modelMatrix = *m_modelsForward[i].modelMatrix;

			mat4 mvp = shadowViewProj * modelMatrix;
			m_shadowShader.SetUniVariable("MVP", mat4x4, &mvp);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].texID);

			m_modelsForward[i].bufferPtr->draw(m_shadowShader.GetShaderProgram());
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	//------------------------------------------------
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
	glCullFace(GL_BACK);
	glDisable(GL_POLYGON_OFFSET_FILL);
	//------------------------------
}

void GraphicDevice::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//--------Uniforms-------------------------------------------------------------------------
	mat4 projectionMatrix = *m_camera->GetProjMatrix();
	mat4 viewMatrix = *m_camera->GetViewMatrix();

	if (m_modelsForward.size() > 0)
	{
		WriteShadowMapDepth();

		glViewport(0, 0, m_clientWidth, m_clientHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//------FORWARD RENDERING--------------------------------------------
		//glEnable(GL_BLEND);

		m_forwardShader.UseProgram();
		m_forwardShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);
		m_forwardShader.SetUniVariable("ViewMatrix", mat4x4, &viewMatrix);
		glm::mat4 invViewMatrix = glm::inverse(viewMatrix);
		m_forwardShader.SetUniVariable("InvViewMatrix", mat4x4, &invViewMatrix);

		mat4 shadowVP = (*m_shadowMap->GetProjectionMatrix()) * (*m_shadowMap->GetViewMatrix());
		m_forwardShader.SetUniVariable("ShadowViewProj", mat4x4, &shadowVP);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_shadowMap->GetDepthTexHandle());

		for (int i = 0; i < m_modelsForward.size(); i++)
		{
			if (m_modelsForward[i].active) // IS MODEL ACTIVE?
			{
				mat4 modelMatrix;
				if (m_modelsForward[i].modelMatrix == NULL)
					modelMatrix = glm::translate(glm::vec3(1));
				else
					modelMatrix = *m_modelsForward[i].modelMatrix;

				mat4 modelViewMatrix = viewMatrix * modelMatrix;
				m_forwardShader.SetUniVariable("ModelViewMatrix", mat4x4, &modelViewMatrix);

				mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
				m_forwardShader.SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].texID);

				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].norID);

				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, m_modelsForward[i].speID);

				m_modelsForward[i].bufferPtr->draw(m_forwardShader.GetShaderProgram());
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		//-------------------------------------------------------------------------
	}
	glViewport(0, 0, m_clientWidth, m_clientHeight);
	glDisable(GL_CULL_FACE);
	// DRAW SKYBOX
	m_skyBoxShader.UseProgram();
	m_skybox->Draw(m_skyBoxShader.GetShaderProgram(), m_camera);
	// -----------

	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	// RENDER VIEWSPACE STUFF
	m_viewspaceShader.UseProgram();
	m_viewspaceShader.SetUniVariable("ProjectionMatrix", mat4x4, &projectionMatrix);

	for (int i = 0; i < m_modelsViewspace.size(); i++)
	{
		if (m_modelsViewspace[i].active) // IS MODEL ACTIVE?
		{
			mat4 modelMatrix;
			if (m_modelsViewspace[i].modelMatrix == NULL)
				modelMatrix = glm::translate(glm::vec3(1));
			else
                modelMatrix = *m_modelsViewspace[i].modelMatrix;
            

			mat4 modelViewMatrix = modelMatrix;
			m_viewspaceShader.SetUniVariable("ModelViewMatrix", mat4x4, &modelViewMatrix);

			mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelViewMatrix)));
			m_viewspaceShader.SetUniVariable("NormalMatrix", mat3x3, &normalMatrix);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].texID);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].norID);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_modelsViewspace[i].speID);

			m_modelsViewspace[i].bufferPtr->draw(m_viewspaceShader.GetShaderProgram());
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	/*if (m_modelsForward.size() > 0)
	{
		float positionData[] = {
			-1.0, -1.0,
			1.0, -1.0,
			1.0, 1.0,
			1.0, 1.0,
			-1.0, 1.0,
			-1.0, -1.0
		};

		m_fullscreen.UseProgram();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_shadowMap->GetDepthTexHandle());

		GLuint buf;
		glGenBuffers(1, &buf);

		glBindBuffer(GL_ARRAY_BUFFER, buf);
		glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), positionData, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDeleteBuffers(1, &buf);
	}*/
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);

	SDL_GL_SwapWindow(m_window);
}

void GraphicDevice::ResizeWindow(int _width, int _height)
{
	// GRAPHIC CARD WORK GROUPS OF 16x16
	/*int x, y;
	x = _width / 16;
	if (x == 0) x = 1;
	y = _height / 16;
	if (y == 0) y = 1;
	m_clientWidth = x * 16;
	m_clientHeight = y * 16;

	std::cout << m_clientWidth << "x" << m_clientHeight << std::endl;*/

	m_clientWidth = _width;
	m_clientHeight = _height;
	SDL_Log("W: %d H: %d", m_clientWidth, m_clientHeight);

	SDL_SetWindowSize(m_window, m_clientWidth, m_clientHeight);
}

bool GraphicDevice::InitSDLWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1){
		SDL_Log( SDL_GetError() );
		return false;
	}

	// WINDOW SETTINGS
	unsigned int	Flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	const char*		Caption = "SDL Window";
	int				PosX = 0;
	int				PosY = 0;

	SDL_DisplayMode dm;
	SDL_GetDisplayMode(0, 0, &dm);
	int				SizeX = dm.w;	//1280
	int				SizeY = dm.h;	//720

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	m_window = SDL_CreateWindow(Caption, PosX, PosY, SizeX, SizeY, Flags);

	if (m_window == NULL){
		SDL_Log( SDL_GetError() );
		return false;
	}

	SDL_GetWindowSize(m_window, &m_clientWidth, &m_clientHeight);
	SDL_Log("W: %d H: %d", m_clientWidth, m_clientHeight);

	m_glContext = SDL_GL_CreateContext(m_window);

	SDL_GL_SetSwapInterval(0);

	return true;
}

bool GraphicDevice::InitShaders()
{
	// Standard forward
	m_forwardShader.InitShaderProgram();
	m_forwardShader.AddShader("content/shaders/AndroidForwardVS.glsl", GL_VERTEX_SHADER);
	m_forwardShader.AddShader("content/shaders/AndroidForwardFS.glsl", GL_FRAGMENT_SHADER);
	m_forwardShader.FinalizeShaderProgram();

	// SkyBox
	m_skyBoxShader.InitShaderProgram();
	m_skyBoxShader.AddShader("content/shaders/AndroidSkyboxShaderVS.glsl", GL_VERTEX_SHADER);
	m_skyBoxShader.AddShader("content/shaders/AndroidSkyboxShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_skyBoxShader.FinalizeShaderProgram();

	// Viewspace shader
	m_viewspaceShader.InitShaderProgram();
	m_viewspaceShader.AddShader("content/shaders/AndroidViewspaceShaderVS.glsl", GL_VERTEX_SHADER);
	m_viewspaceShader.AddShader("content/shaders/AndroidViewspaceShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_viewspaceShader.FinalizeShaderProgram();

	// ShadowShader geometry
	m_shadowShader.InitShaderProgram();
	m_shadowShader.AddShader("content/shaders/AndroidShadowShaderVS.glsl", GL_VERTEX_SHADER);
	m_shadowShader.AddShader("content/shaders/AndroidShadowShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_shadowShader.FinalizeShaderProgram();

	//m_fullscreen
	/*m_fullscreen.InitShaderProgram();
	m_fullscreen.AddShader("content/shaders/AndroidFullscreenVS.glsl", GL_VERTEX_SHADER);
	m_fullscreen.AddShader("content/shaders/AndroidFullscreenFS.glsl", GL_FRAGMENT_SHADER);
	m_fullscreen.FinalizeShaderProgram();*/

	return true;
}

bool GraphicDevice::InitBuffers()
{
	//Skybox shader
	m_skyBoxShader.CheckUniformLocation("cubemap", 1);

	//Fullscreen shader
	//m_fullscreen.CheckUniformLocation("sampler", 0);

	return true;
}

bool GraphicDevice::InitSkybox()
{
	int w, h;
	GLuint texHandle = TextureLoader::LoadCubeMap("content/textures/skybox", GL_TEXTURE1, w, h);
	if (texHandle < 0)
		return false;

	m_skyBoxShader.UseProgram();
	GLuint loc = glGetAttribLocation(m_skyBoxShader.GetShaderProgram(), "VertexPoint");
	m_skybox = new SkyBox(texHandle, m_camera->GetFarPlane(), loc);

	return true;
}

void GraphicDevice::BufferPointlights(int _nrOfLights, float **_lightPointers)
{

}

void GraphicDevice::BufferDirectionalLight(float *_lightPointer)
{
	//direction, intensity, color
    
    if (_lightPointer)
    {
        m_dirLightDirection = vec3(_lightPointer[0], _lightPointer[1], _lightPointer[2]);
        vec3 intens = vec3(_lightPointer[3], _lightPointer[4], _lightPointer[5]);
        vec3 color = vec3(_lightPointer[6], _lightPointer[7], _lightPointer[8]);
        
        m_forwardShader.SetUniVariable("dirlight.Direction", vector3, &m_dirLightDirection);
        m_forwardShader.SetUniVariable("dirlight.Intensity", vector3, &intens);
        m_forwardShader.SetUniVariable("dirlight.Color", vector3, &color);
    }
    else
    {
        vec3 zero = vec3(0.0f);

        m_forwardShader.SetUniVariable("dirlight.Intensity", vector3, &zero);
        m_forwardShader.SetUniVariable("dirlight.Color", vector3, &zero);
    }
	

	m_dirLightDirection = vec3(_lightPointer[0], _lightPointer[1], _lightPointer[2]);
	m_shadowMap->UpdateViewMatrix(vec3(8.0f, 0.0f, 8.0f) - (10.0f*normalize(m_dirLightDirection)), vec3(8.0f, 0.0f, 8.0f));
}

void GraphicDevice::CreateShadowMap()
{
	int resolution = 1024*2;
	m_dirLightDirection = vec3(0.0f, -1.0f, 1.0f);
	vec3 midMap = vec3(8.0f, 0.0f, 8.0f);
	vec3 lightPos = midMap - (10.0f*normalize(m_dirLightDirection));
	m_shadowMap = new ShadowMap(lightPos, lightPos + normalize(m_dirLightDirection), resolution);
	m_shadowMap->CreateShadowMapTexture(GL_TEXTURE0);

	m_forwardShader.UseProgram();
	m_forwardShader.SetUniVariable("BiasMatrix", mat4x4, m_shadowMap->GetBiasMatrix());
	m_forwardShader.CheckUniformLocation("ShadowDepthTex", 0);

	m_shadowShader.CheckUniformLocation("diffuseTex", 1);
}

bool GraphicDevice::RenderSimpleText(std::string _text, int _x, int _y)
{
	return true;
}
void GraphicDevice::SetSimpleTextColor(float _r, float _g, float _b, float _a)
{
}
void GraphicDevice::SetDisco()
{
}
void GraphicDevice::ToggleSimpleText(bool _render)
{
}
void GraphicDevice::ToggleSimpleText()
{
}

bool GraphicDevice::PreLoadModel(std::string _dir, std::string _file, int _renderType)
{
	Shader *shaderPtr = &m_forwardShader;
	m_forwardShader.UseProgram();

	// Import Object
	//ObjectData obj = AddObject(_dir, _file);
	ObjectData obj = ModelLoader::importObject(_dir, _file);

	// Import Texture
	GLuint texture = AddTexture(obj.text, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	// Import Normal map
	GLuint normal = AddTexture(obj.norm, GL_TEXTURE2);
	shaderPtr->CheckUniformLocation("normalTex", 2);

	// Import Specc Glow map
	GLuint specular = AddTexture(obj.spec, GL_TEXTURE3);
	shaderPtr->CheckUniformLocation("specularTex", 3);

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh, shaderPtr);

	return true;
}
int GraphicDevice::LoadModel(std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType)
{
	int modelID = m_modelIDcounter;
	m_modelIDcounter++;

	Shader *shaderPtr = NULL;

	if (_renderType == RENDER_FORWARD)
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
	}
	else if (_renderType == RENDER_VIEWSPACE)
	{
		shaderPtr = &m_viewspaceShader;
		m_viewspaceShader.UseProgram();
	}
	/*else if (_renderType == RENDER_INTERFACE)
	{
		shaderPtr = &m_interfaceShader;
		m_interfaceShader.UseProgram();
	}*/
	else
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
		//SDL_Log("Deferred requested. Selecting FORWARD");
	}

	// Import Object
	ObjectData obj = ModelLoader::importObject(_dir, _file);

	// Import Texture
	GLuint texture = AddTexture(obj.text, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	// Import Normal map
	GLuint normal = AddTexture(obj.norm, GL_TEXTURE2);
	shaderPtr->CheckUniformLocation("normalTex", 2);

	// Import Specc Glow map
	GLuint specular = AddTexture(obj.spec, GL_TEXTURE3);
	shaderPtr->CheckUniformLocation("specularTex", 3);

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh, shaderPtr);

	Model model = Model(mesh, texture, normal, specular, modelID, true, _matrixPtr); // plus modelID o matrixPointer, active


	// Push back the model
	if (_renderType == RENDER_FORWARD)
		m_modelsForward.push_back(model);
	else if (_renderType == RENDER_VIEWSPACE)
		m_modelsViewspace.push_back(model);
	else
		m_modelsForward.push_back(model);

	//else if (_renderType == RENDER_INTERFACE)
	//	m_modelsInterface.push_back(model);

	return modelID;
}
bool GraphicDevice::RemoveModel(int _id)
{
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].id == _id)
		{
			m_modelsForward.erase(m_modelsForward.begin() + i);

			return true;
		}
	}
	for (int i = 0; i < m_modelsViewspace.size(); i++)
	{
		if (m_modelsViewspace[i].id == _id)
		{
			m_modelsViewspace.erase(m_modelsViewspace.begin() + i);

			return true;
		}
	}

	return false;
}
bool GraphicDevice::ActiveModel(int _id, bool _active)
{
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].id == _id)
		{
			m_modelsForward[i].active = _active;
			return true;
		}
	}
	return false;
}
bool GraphicDevice::ChangeModelTexture(int _id, std::string _fileDir, int _textureType)
{
	//// Model Instance
	//Instance instance;
	//// Temp Model
	//Model model;

	//bool found = false;
	//int renderType;

	//// Find model Instance
	//for (int i = 0; i < m_modelsForward.size(); i++)
	//{
	//	for (int j = 0; j < m_modelsForward[i].instances.size(); j++)
	//	{
	//		if (m_modelsForward[i].instances[j].id == _id)
	//		{
	//			instance = m_modelsForward[i].instances[j];
	//			model = Model(
	//				m_modelsForward[i].bufferPtr,
	//				m_modelsForward[i].texID,
	//				m_modelsForward[i].norID,
	//				m_modelsForward[i].speID
	//				);
	//			found = true;
	//			renderType = RENDER_FORWARD;
	//			m_modelsForward[i].instances.erase(m_modelsForward[i].instances.begin() + j);
	//			if (m_modelsForward[i].instances.size() == 0)
	//				m_modelsForward.erase(m_modelsForward.begin() + i);
	//		}
	//		if (found) break;
	//	}
	//	if (found) break;
	//}
	//// Didn't we find it return false
	//if (!found) return false;

	//// Set new Texture to TextureType
	//if (_textureType == TEXTURE_DIFFUSE)
	//{
	//	model.texID = AddTexture(_fileDir, GL_TEXTURE1);
	//	m_forwardShader.CheckUniformLocation("diffuseTex", 1);
	//}
	//else if (_textureType == TEXTURE_NORMAL)
	//{
	//	model.norID = AddTexture(_fileDir, GL_TEXTURE2);
	//	m_forwardShader.CheckUniformLocation("normalTex", 2);
	//}
	//else if (_textureType == TEXTURE_SPECULAR)
	//{
	//	model.speID = AddTexture(_fileDir, GL_TEXTURE3);
	//	m_forwardShader.CheckUniformLocation("specularTex", 3);
	//}

	//// Check if our new Model type already exists and add instance
	//for (int i = 0; i < m_modelsForward.size(); i++)
	//{
	//	if (m_modelsForward[i] == model)
	//	{
	//		m_modelsForward[i].instances.push_back(instance);
	//		return true;
	//	}
	//}

	//// Nothing found. Let's make a new Model type
	//model.instances.push_back(instance);
	//// Push back the model
	//if (renderType == RENDER_DEFERRED)
	//	m_modelsDeferred.push_back(model);
	//else if (renderType == RENDER_FORWARD)
	//	m_modelsForward.push_back(model);

	return true;
}
bool GraphicDevice::ChangeModelNormalMap(int _id, std::string _fileDir)
{
	return ChangeModelTexture(_id, _fileDir, TEXTURE_NORMAL);
}
bool GraphicDevice::ChangeModelSpecularMap(int _id, std::string _fileDir)
{
	return ChangeModelTexture(_id, _fileDir, TEXTURE_SPECULAR);
}

Buffer* GraphicDevice::AddMesh(std::string _fileDir, Shader *_shaderProg)
{
	for (std::map<const std::string, Buffer*>::iterator it = m_meshs.begin(); it != m_meshs.end(); it++)
	{		
		if (it->first == _fileDir)
			return it->second;
	}

	ModelExporter modelExporter;
	modelExporter.OpenFileForRead(_fileDir.c_str());
	std::vector<float> positionData = modelExporter.ReadDataFromFile();
	std::vector<float> normalData = modelExporter.ReadDataFromFile();
	std::vector<float> tanData = modelExporter.ReadDataFromFile();
	std::vector<float> bitanData = modelExporter.ReadDataFromFile();
	std::vector<float> texCoordData = modelExporter.ReadDataFromFile();
	modelExporter.CloseFile();

	Buffer* retbuffer = new Buffer();

	std::map<GLuint, GLuint> vpLocs, vnLocs, tanLocs, bitanLocs, tcLocs;
	vpLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexPosition");
	vpLocs[m_shadowShader.GetShaderProgram()]	 = glGetAttribLocation(m_shadowShader.GetShaderProgram(), "VertexPosition");

	vnLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexNormal");
	vnLocs[m_shadowShader.GetShaderProgram()]	 = glGetAttribLocation(m_shadowShader.GetShaderProgram(), "VertexNormal");

	tanLocs[m_forwardShader.GetShaderProgram()]	  = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexTangent");
	tanLocs[m_shadowShader.GetShaderProgram()]	  = glGetAttribLocation(m_shadowShader.GetShaderProgram(), "VertexTangent");

	bitanLocs[m_forwardShader.GetShaderProgram()]	= glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexBiTangent");
	bitanLocs[m_shadowShader.GetShaderProgram()]	= glGetAttribLocation(m_shadowShader.GetShaderProgram(), "VertexBiTangent");

	tcLocs[m_forwardShader.GetShaderProgram()]	 = glGetAttribLocation(m_forwardShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_viewspaceShader.GetShaderProgram()] = glGetAttribLocation(m_viewspaceShader.GetShaderProgram(), "VertexTexCoord");
	tcLocs[m_shadowShader.GetShaderProgram()]	 = glGetAttribLocation(m_shadowShader.GetShaderProgram(), "VertexTexCoord");

	_shaderProg->UseProgram();
	BufferData bufferData[] =
	{
		{ vpLocs,	 3, GL_FLOAT, (const GLvoid*)positionData.data(), (GLsizeiptr)(positionData.size() * sizeof(float)) },
		{ vnLocs,	 3, GL_FLOAT, (const GLvoid*)normalData.data(), (GLsizeiptr)(normalData.size()   * sizeof(float)) },
		{ tanLocs,	 3, GL_FLOAT, (const GLvoid*)tanData.data(), (GLsizeiptr)(tanData.size()   * sizeof(float)) },
		{ bitanLocs, 3, GL_FLOAT, (const GLvoid*)bitanData.data(), (GLsizeiptr)(bitanData.size()   * sizeof(float)) },
		{ tcLocs,	 2, GL_FLOAT, (const GLvoid*)texCoordData.data(), (GLsizeiptr)(texCoordData.size() * sizeof(float)) }
	};

	retbuffer->init(bufferData, sizeof(bufferData) / sizeof(bufferData[0]), _shaderProg->GetShaderProgram());
	retbuffer->setCount((int)positionData.size() / 3);
	
	m_meshs.insert(std::pair<const std::string, Buffer*>(_fileDir, retbuffer));

	return retbuffer;
}
GLuint GraphicDevice::AddTexture(std::string _fileDir, GLenum _textureSlot)
{
	for (std::map<const std::string, GLuint>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		if (it->first == _fileDir)
			return it->second;
	}
	int texSizeX, texSizeY;
	//m_deferredShader1.UseProgram();
	GLuint texture = TextureLoader::LoadTexture(_fileDir.c_str(), _textureSlot, texSizeX, texSizeY);
	m_textures.insert(std::pair<const std::string, GLenum>(_fileDir, texture));
	return texture;
}
//ObjectData GraphicDevice::AddObject(std::string _file, std::string _dir)
//{
//	std::string fileDir = _dir;
//	fileDir.append(_file);
//	for (std::map<const std::string, ObjectData>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
//	{
//		if (it->first == fileDir)
//			return it->second;
//	}
//	ObjectData obj = ModelLoader::importObject(_dir, _file);
//}

void GraphicDevice::Clear()
{
  m_modelIDcounter = 0;
  
  m_modelsForward.clear();
  m_modelsViewspace.clear();

  float **tmpPtr = new float*[1];
  BufferPointlights(0, tmpPtr);
  delete tmpPtr;
}

int GraphicDevice::AddFont(const std::string& filepath, int size)
{
	return m_sdlTextRenderer.AddFont(filepath, size);
}

void GraphicDevice::CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size)
{
	assert(m_textures.find(textureName) == m_textures.end());
	SDL_Surface* surface = m_sdlTextRenderer.CreateTextSurface(textString, fontIndex, color);
	if (size.x > 0)
		surface->w = size.x;
	if (size.y > 0)
		surface->h = size.y;
	m_forwardShader.UseProgram();
	GLuint texture = TextureLoader::LoadTexture(surface, GL_TEXTURE1);
	m_textures[textureName] = texture;
	SDL_FreeSurface(surface);
}

void GraphicDevice::CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size)
{
	assert(m_textures.find(textureName) == m_textures.end());
	SDL_Surface* surface = m_sdlTextRenderer.CreateWrappedTextSurface(textString, fontIndex, color, wrapLength);
	if (size.x > 0)
		surface->w = size.x;
	if (size.y > 0)
		surface->h = size.y;
	m_forwardShader.UseProgram();
	GLuint texture = TextureLoader::LoadTexture(surface, GL_TEXTURE1);
	m_textures[textureName] = texture;
	SDL_FreeSurface(surface);
}
