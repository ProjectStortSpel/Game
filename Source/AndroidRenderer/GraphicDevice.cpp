/* ANDROID VERSION */
#include "GraphicDevice.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "TextureLoader.h"

#include "ModelExporter.h"

using namespace Renderer;
using namespace glm;

GraphicDevice::GraphicDevice()
{
	m_SDLinitialized = false;
	m_pointlightsPtr = 0;
	m_particleID = 0;
	m_elapsedTime = 0.0f;
	
	m_useAnimations = true;
}

GraphicDevice::GraphicDevice(Camera _camera)
{
	m_camera = new Camera(_camera);
	m_SDLinitialized = true;
	m_pointlightsPtr = 0;
	m_particleID = 0;
	m_elapsedTime = 0.0f;
	
	m_useAnimations = true;
}

GraphicDevice::~GraphicDevice()
{
	delete(m_camera);
	delete(m_skybox);
	delete(m_skyboxClouds);

	if (m_pointlightsPtr)
		delete [] m_pointlightsPtr;

	for (std::map<int, ParticleEffect*>::iterator it = m_particleEffects.begin(); it != m_particleEffects.end(); ++it)
		delete(it->second);

	for (std::map<std::string, Shader*>::iterator it = m_particleShaders.begin(); it != m_particleShaders.end(); ++it)
		delete(it->second);

	for (std::map<const std::string, GLuint>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
		glDeleteTextures(1, &(it->second));

	for (int i = 0; i < m_surfaces.size(); i++)
		delete(m_surfaces[i].second);

#ifdef __ANDROID__
	glDeleteBuffers(1, &m_fullscreenQuadBuffer);
#endif
	SDL_GL_DeleteContext(m_glContext);
	// Close and destroy the window
	SDL_DestroyWindow(m_window);
	// Clean up
	//SDL_Quit();
	//SDL_Log("Graphics D E S T R U C T O R");
}

void GraphicDevice::InitFBO()
{
	GLint oldFBO;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);

	// --------Create the depth texture-------
	//glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &m_depthBuf);
	glBindTexture(GL_TEXTURE_2D, m_depthBuf);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_framebufferWidth, m_framebufferHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
	// ---------------------------------------

	SDL_Log("InitFBO     m_framebufferWidth %d,  m_framebufferHeight: %d ", m_framebufferWidth, m_framebufferHeight);

	// Output Image
	glGenTextures(1, &m_outputImage);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_outputImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_framebufferWidth, m_framebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); //level = 1?  GL_RGBA32F ?
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// Create and bind the FBO
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// Attach the images to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuf, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_outputImage, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		SDL_Log("Framebuffer complete.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		SDL_Log("[ERROR] Framebuffer incomplete: Attachment is NOT complete.");
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		SDL_Log("[ERROR] Framebuffer incomplete: No image is attached to FBO.");
		break;

	case GL_FRAMEBUFFER_UNSUPPORTED:
		SDL_Log("[ERROR] Unsupported by FBO implementation.");
		break;

	default:
		SDL_Log("[ERROR] Unknown error.");
		break;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
}

void GraphicDevice::InitStandardBuffers()
{
	//Skybox shader
	m_skyBoxShader.CheckUniformLocation("cubemap", 1);
	m_skyboxCloudsShader.CheckUniformLocation("cubemap", 2);

	//Particle shaders
	for (std::map<std::string, Shader*>::iterator it = m_particleShaders.begin(); it != m_particleShaders.end(); ++it)
	{
		it->second->CheckUniformLocation("diffuseTex", 1);
	}

	//Fullscreen shader
	m_fullscreen.CheckUniformLocation("sampler", 4);

#ifdef __ANDROID__
	CreateFullscreenQuad();
#endif
}

void GraphicDevice::InitStandardShaders()
{
	// SkyBox
	m_skyBoxShader.InitShaderProgram();
	m_skyBoxShader.AddShader("content/shaders/android/AndroidSkyboxShaderVS.glsl", GL_VERTEX_SHADER);
	m_skyBoxShader.AddShader("content/shaders/android/AndroidSkyboxShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_skyBoxShader.FinalizeShaderProgram();

	m_skyboxCloudsShader.InitShaderProgram();
	m_skyboxCloudsShader.AddShader("content/shaders/android/AndroidSkyboxShaderVS.glsl", GL_VERTEX_SHADER);
	m_skyboxCloudsShader.AddShader("content/shaders/android/AndroidSkyboxShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_skyboxCloudsShader.FinalizeShaderProgram();

	// Viewspace shader
	m_viewspaceShader.InitShaderProgram();
	m_viewspaceShader.AddShader("content/shaders/android/AndroidViewspaceShaderVS.glsl", GL_VERTEX_SHADER);
	m_viewspaceShader.AddShader("content/shaders/android/AndroidViewspaceShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_viewspaceShader.FinalizeShaderProgram();

	// Interface shader
	m_interfaceShader.InitShaderProgram();
	m_interfaceShader.AddShader("content/shaders/android/AndroidInterfaceVS.glsl", GL_VERTEX_SHADER);
	m_interfaceShader.AddShader("content/shaders/android/AndroidInterfaceFS.glsl", GL_FRAGMENT_SHADER);
	m_interfaceShader.FinalizeShaderProgram();

	// Particle shaders
	m_particleShaders["smoke"] = new Shader();
	m_particleShaders["smoke"]->InitShaderProgram();
	m_particleShaders["smoke"]->AddShader("content/shaders/android/AndroidSmokeShaderVS.glsl", GL_VERTEX_SHADER);
	m_particleShaders["smoke"]->AddShader("content/shaders/android/AndroidSmokeShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_particleShaders["smoke"]->FinalizeShaderProgram();

	m_particleShaders["fire"] = new Shader();
	m_particleShaders["fire"]->InitShaderProgram();
	m_particleShaders["fire"]->AddShader("content/shaders/android/AndroidFireShaderVS.glsl", GL_VERTEX_SHADER);
	m_particleShaders["fire"]->AddShader("content/shaders/android/AndroidFireShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_particleShaders["fire"]->FinalizeShaderProgram();

	m_particleShaders["waterfall"] = new Shader();
	m_particleShaders["waterfall"]->InitShaderProgram();
	m_particleShaders["waterfall"]->AddShader("content/shaders/android/AndroidWaterfallShaderVS.glsl", GL_VERTEX_SHADER);
	m_particleShaders["waterfall"]->AddShader("content/shaders/android/AndroidWaterfallShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_particleShaders["waterfall"]->FinalizeShaderProgram();

	m_particleShaders["waterspawn"] = new Shader();
	m_particleShaders["waterspawn"]->InitShaderProgram();
	m_particleShaders["waterspawn"]->AddShader("content/shaders/android/AndroidSmokeShaderVS.glsl", GL_VERTEX_SHADER);
	m_particleShaders["waterspawn"]->AddShader("content/shaders/android/AndroidSmokeShaderFS.glsl", GL_FRAGMENT_SHADER);
	m_particleShaders["waterspawn"]->FinalizeShaderProgram();
	
	//m_fullscreen
	m_fullscreen.InitShaderProgram();
	m_fullscreen.AddShader("content/shaders/android/AndroidFullscreenVS.glsl", GL_VERTEX_SHADER);
	m_fullscreen.AddShader("content/shaders/android/AndroidFullscreenFS.glsl", GL_FRAGMENT_SHADER);
	m_fullscreen.FinalizeShaderProgram();
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

void GraphicDevice::GetWindowSize(int &x, int &y)
{ 
	x = m_clientWidth; 
	y = m_clientHeight; 
}

void GraphicDevice::GetFramebufferSize(int &x, int &y)
{
#ifdef __ANDROID__
	x = m_framebufferWidth;
	y = m_framebufferHeight;
#else
	x = m_clientWidth;
	y = m_clientHeight;
#endif
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

bool GraphicDevice::InitSkybox()
{
	int w, h;
	m_skyBoxShader.UseProgram();
	GLuint texHandle = TextureLoader::LoadCubeMap("content/textures/skybox", GL_TEXTURE1, w, h);
	if (texHandle < 0)
		return false;

	GLuint loc = glGetAttribLocation(m_skyBoxShader.GetShaderProgram(), "VertexPoint");
	m_skybox = new SkyBox(texHandle, m_camera->GetFarPlane(), loc, 0.0f);

	m_skyboxCloudsShader.UseProgram();
	texHandle = TextureLoader::LoadCubeMap("content/textures/clouds", GL_TEXTURE2, w, h);
	if (texHandle < 0)
		return false;

	loc = glGetAttribLocation(m_skyboxCloudsShader.GetShaderProgram(), "VertexPoint");
	m_skyboxClouds = new SkyBox(texHandle, m_camera->GetFarPlane(), loc, 0.010f);

	return true;
}


bool GraphicDevice::SetAnimation(int _modelId, int _animId, float _frameTime)
{
	std::vector<AModel> *modelList = &m_modelsAnimated;
	for (int i = 0; i < (*modelList).size(); i++)
	{
		if ((*modelList)[i].id == _modelId)
		{
			if ((*modelList)[i].SetAnimation(_animId))
			{
				(*modelList)[i].SetFrameTime(_frameTime);
				return true;
			}	
		}
	}
	return false;
}
glm::mat4 GraphicDevice::GetJointMatrix(int _modelId, int _jointId)
{
	std::vector<AModel> *modelList = &m_modelsAnimated;
	for (int i = 0; i < (*modelList).size(); i++)
	{
		if ((*modelList)[i].id == _modelId)
		{
			if ((*modelList)[i].anim.size() > _jointId)
				return (*modelList)[i].anim[_jointId] * glm::inverse((*modelList)[i].joints[_jointId]);
			else
				break;
		}
	}
	return glm::mat4(1.0f);
}

GLuint GraphicDevice::AddTexture(std::string _fileDir, GLenum _textureSlot)
{
    //printf("fileDir: %s\n", _fileDir.c_str());
	for (std::map<const std::string, GLuint>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
	{
		if (it->first == _fileDir)
			return it->second;
	}
	int texSizeX, texSizeY;
	GLuint texture = TextureLoader::LoadTexture(_fileDir.c_str(), _textureSlot, texSizeX, texSizeY);
	m_textures.insert(std::pair<const std::string, GLenum>(_fileDir, texture));
	return texture;
}

int GraphicDevice::AddFont(const std::string& filepath, int size)
{
	return TextRenderer::AddFont(filepath, size);
}

float GraphicDevice::CreateTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, glm::ivec2 size)
{
	SDL_Surface* surface = TextRenderer::CreateTextSurface(textString, fontIndex, color);
	if (size.x > 0)
		surface->w = size.x;
	if (size.y > 0)
		surface->h = size.y;
	m_surfaces.push_back(std::pair<std::string, SDL_Surface*>(textureName, surface));
	return (float)surface->w / (float)surface->h;
}

void GraphicDevice::CreateWrappedTextTexture(const std::string& textureName, const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength, glm::ivec2 size)
{
	SDL_Surface* surface = TextRenderer::CreateWrappedTextSurface(textString, fontIndex, color, wrapLength);
	if (size.x > 0)
		surface->w = size.x;
	if (size.y > 0)
		surface->h = size.y;
	m_surfaces.push_back(std::pair<std::string, SDL_Surface*>(textureName, surface));
}

void GraphicDevice::BufferSurfaces()
{
	for (std::pair<std::string, SDL_Surface*> surface : m_surfaces)
	{
		int oldTexture = -1;
		if (m_textures.find(surface.first) != m_textures.end())
		{
			oldTexture = m_textures[surface.first];
			glDeleteTextures(1, &m_textures[surface.first]);
		}
		GLuint texture = TextureLoader::LoadTexture(surface.second, GL_TEXTURE1);
		m_textures[surface.first] = texture;
		SDL_FreeSurface(surface.second);
		if (oldTexture != -1)
		{
			for (Model& m : m_modelsForward)
				if (m.texID == oldTexture)
					m.texID = texture;
			for (Model& m : m_modelsViewspace)
				if (m.texID == oldTexture)
					m.texID = texture;
			for (Model& m : m_modelsInterface)
				if (m.texID == oldTexture)
					m.texID = texture;
		}
	}
	m_surfaces.clear();
}

void GraphicDevice::BufferModelTextures()
{
	for (ModelTexture modelTexture : m_modelTextures)
	{
		BufferModelTexture(modelTexture.id, modelTexture.textureName, modelTexture.textureType);
	}
	m_modelTextures.clear();
}

bool GraphicDevice::BufferModelTexture(int _id, std::string _fileDir, int _textureType)
{
  // Temp Model
	Model model;

	bool found = false;
	int renderType;

	// Find model
	for (int i = 0; i < m_modelsForward.size(); i++)
	{
		if (m_modelsForward[i].id == _id)
		{
			model = Model(
				m_modelsForward[i].bufferPtr,
				m_modelsForward[i].texID,
				m_modelsForward[i].norID,
				m_modelsForward[i].speID,
				m_modelsForward[i].id,
				m_modelsForward[i].active,
				m_modelsForward[i].modelMatrix,
				m_modelsForward[i].color,
				m_modelsForward[i].castShadow
				);
			found = true;
			renderType = RENDER_FORWARD;
			m_modelsForward.erase(m_modelsForward.begin() + i);
		}
		if (found) break;
	}
	if (!found)
	{
		for (int i = 0; i < m_modelsViewspace.size(); i++)
		{
			if (m_modelsViewspace[i].id == _id)
			{
				model = Model(
					m_modelsViewspace[i].bufferPtr,
					m_modelsViewspace[i].texID,
					m_modelsViewspace[i].norID,
					m_modelsViewspace[i].speID,
					m_modelsViewspace[i].id,
					m_modelsViewspace[i].active,
					m_modelsViewspace[i].modelMatrix,
					m_modelsViewspace[i].color,
					false
					);
				found = true;
				renderType = RENDER_VIEWSPACE;
				m_modelsViewspace.erase(m_modelsViewspace.begin() + i);
			}
			if (found) break;
		}
	}
	if (!found)
	{
		for (int i = 0; i < m_modelsInterface.size(); i++)
		{
			if (m_modelsInterface[i].id == _id)
			{
				model = Model(
					m_modelsInterface[i].bufferPtr,
					m_modelsInterface[i].texID,
					m_modelsInterface[i].norID,
					m_modelsInterface[i].speID,
					m_modelsInterface[i].id,
					m_modelsInterface[i].active,
					m_modelsInterface[i].modelMatrix,
					m_modelsInterface[i].color,
					false
					);
				found = true;
				renderType = RENDER_INTERFACE;
				m_modelsInterface.erase(m_modelsInterface.begin() + i);
			}
			if (found) break;
		}
	}
	// Didn't we find it return false
	if (!found) return false;

	// Set new Texture to TextureType
	if (_textureType == TEXTURE_DIFFUSE)
	{
		model.texID = AddTexture(_fileDir, GL_TEXTURE1);
		//m_deferredShader1.CheckUniformLocation("diffuseTex", 1);
	}
	else if (_textureType == TEXTURE_NORMAL)
	{
		model.norID = AddTexture(_fileDir, GL_TEXTURE2);
		//m_deferredShader1.CheckUniformLocation("normalTex", 2);
	}
	else if (_textureType == TEXTURE_SPECULAR)
	{
		model.speID = AddTexture(_fileDir, GL_TEXTURE3);
		//m_deferredShader1.CheckUniformLocation("specularTex", 3);
	}

	// Check if our new Model type already exists and add instance
	if (renderType == RENDER_FORWARD)
	{
		m_modelsForward.push_back(model);
		return true;
	}
	else if (renderType == RENDER_VIEWSPACE)
	{
		m_modelsViewspace.push_back(model);
		return true;
	}
	else if (renderType == RENDER_INTERFACE)
	{
		m_modelsInterface.push_back(model);
		return true;
	}
	else
		SDL_Log("Change texture to incorrect renderType.");

	return false;
}

void GraphicDevice::BufferModel(int _modelId, ModelToLoad* _modelToLoad)
{
	Shader *shaderPtr = NULL;

	if (_modelToLoad->RenderType == RENDER_FORWARD)
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_VIEWSPACE)
	{
		shaderPtr = &m_viewspaceShader;
		m_viewspaceShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_INTERFACE)
	{
		shaderPtr = &m_interfaceShader;
		m_interfaceShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_RIVERWATER)
	{
		shaderPtr = &m_riverShader;
		m_riverShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_RIVERWATER_CORNER)
	{
		shaderPtr = &m_riverCornerShader;
		m_riverCornerShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == 0)
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
		//SDL_Log("Deferred requested. Selecting FORWARD");
	}
	else
	{
		return;
	}
	// Import Object
	ObjectData obj = ModelLoader::importObject(_modelToLoad->Dirs, _modelToLoad->File);
	
	if (obj.animated && m_useAnimations)
	{
		BufferAModel(_modelId, _modelToLoad);
		return;
	}

	// Import Texture
	GLuint texture = AddTexture(obj.text, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	GLuint normal, specular;

	if (_modelToLoad->RenderType != RENDER_INTERFACE)
	{
		// Import Normal map
		normal = AddTexture(obj.norm, GL_TEXTURE2);
		shaderPtr->CheckUniformLocation("normalTex", 2);

		// Import Specc Glow map
		specular = AddTexture(obj.spec, GL_TEXTURE3);
		shaderPtr->CheckUniformLocation("specularTex", 3);
	}

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh, shaderPtr);

	Model model = Model(mesh, texture, normal, specular, _modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color, _modelToLoad->CastShadow); // plus modelID o matrixPointer, active


	// Push back the model
	if (_modelToLoad->RenderType == RENDER_FORWARD)
		m_modelsForward.push_back(model);
	else if (_modelToLoad->RenderType == RENDER_VIEWSPACE)
		m_modelsViewspace.push_back(model);
	else if (_modelToLoad->RenderType == RENDER_INTERFACE)
		m_modelsInterface.push_back(model);
	else if (_modelToLoad->RenderType == RENDER_RIVERWATER)
		m_modelsWater.push_back(model);
	else if (_modelToLoad->RenderType == RENDER_RIVERWATER_CORNER)
		m_modelsWaterCorners.push_back(model);
	else
		m_modelsForward.push_back(model);
}

void GraphicDevice::BufferAModel(int _modelId, ModelToLoad* _modelToLoad)
{
	ObjectData obj = ModelLoader::importObject(_modelToLoad->Dirs, _modelToLoad->File);

	Shader *shaderPtr = &m_animationShader;
	std::vector<AModel> *modelList = &m_modelsAnimated;

	// Import Mesh
	Buffer* mesh = AddMesh(obj.mesh, shaderPtr, true);
	
	SDL_Log("Buffer AModel, Tex: %s", obj.text.c_str());

	// Import Texture
	GLuint texture = AddTexture(obj.text, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	// Import Normal map
	GLuint normal = AddTexture(obj.norm, GL_TEXTURE2);
	shaderPtr->CheckUniformLocation("normalTex", 2);

	// Import Specc Glow map
	GLuint specular = AddTexture(obj.spec, GL_TEXTURE3);
	shaderPtr->CheckUniformLocation("specularTex", 3);

	AModel model = AModel(_modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color, mesh, texture, normal, specular);

	// Import Skeleton
	std::vector<JointData> joints = ModelLoader::importJoints(obj.joints);

	// Add skeleton
	for (int i = 0; i < joints.size(); i++)
	{
		model.joints.push_back(joints[i].mat);
	}

	// Add animation base
	for (int i = 0; i < joints.size(); i++)
	{
		int index = i;//joints.size() - i - 1;
		model.animation.push_back(Joint(joints[index].parent));
		model.anim.push_back(joints[index].mat);
		model.extra.push_back(mat4(1));
	}

	// Import Animations
	for (int i = 0; i < obj.anim.size(); i++)
	{
		std::vector<AnimData> anim = ModelLoader::importAnimation(obj.anim[i]);
		for (int j = 0; j < anim.size(); j++)
		{
			model.AddKeyFrame(obj.anim[i], anim[j].frame, anim[j].joint, anim[j].mat);
		}
	}

	// Pre-calc frames
	model.PreCalculateAnimations();

	// Push back the model
	model.name = _modelToLoad->File;
	model.name.erase(model.name.end() - 7, model.name.end());
	modelList->push_back(model);
}

void GraphicDevice::BufferModel(int _modelId, ModelToLoadFromSource* _modelToLoad)
{
	// Temporary fix for water
	if (_modelToLoad->RenderType == 5 || _modelToLoad->RenderType == 6)
		_modelToLoad->RenderType = 0;
  
	Shader *shaderPtr = NULL;

	if (_modelToLoad->RenderType == RENDER_FORWARD)
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_VIEWSPACE)
	{
		shaderPtr = &m_viewspaceShader;
		m_viewspaceShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == RENDER_INTERFACE)
	{
		shaderPtr = &m_interfaceShader;
		m_interfaceShader.UseProgram();
	}
	else if (_modelToLoad->RenderType == 0)
	{
		shaderPtr = &m_forwardShader;
		m_forwardShader.UseProgram();
		//SDL_Log("Deferred requested. Selecting FORWARD");
	}
	else
	{
		return;
	}

	// Import Texture
	GLuint texture = AddTexture(_modelToLoad->diffuseTextureFilepath, GL_TEXTURE1);
	shaderPtr->CheckUniformLocation("diffuseTex", 1);

	GLuint normal, specular;

	if (_modelToLoad->RenderType != RENDER_INTERFACE)
	{
		// Import Normal map
		normal = AddTexture(_modelToLoad->normalTextureFilepath, GL_TEXTURE2);
		shaderPtr->CheckUniformLocation("normalTex", 2);

		// Import Specc Glow map
		specular = AddTexture(_modelToLoad->specularTextureFilepath, GL_TEXTURE3);
		shaderPtr->CheckUniformLocation("specularTex", 3);
	}

	// Import Mesh
	Buffer* mesh = AddMesh(_modelToLoad, shaderPtr);

	Model model = Model(mesh, texture, normal, specular, _modelId, true, _modelToLoad->MatrixPtr, _modelToLoad->Color, _modelToLoad->CastShadow); // plus modelID o matrixPointer, active


	// Push back the model
	if (_modelToLoad->RenderType == RENDER_FORWARD)
		m_modelsForward.push_back(model);
	else if (_modelToLoad->RenderType == RENDER_VIEWSPACE)
		m_modelsViewspace.push_back(model);
	else if (_modelToLoad->RenderType == RENDER_INTERFACE)
		m_modelsInterface.push_back(model);
	else
		m_modelsForward.push_back(model);
}

void GraphicDevice::BufferModels()
{
	for (auto pair : m_staticModelsToLoad)
		BufferStaticModel(pair);
	for (auto pair : m_modelsToLoad)
	{
		BufferModel(pair.first, pair.second);
		delete(pair.second);
	}
	for (auto pair : m_modelsToLoadFromSource)
	{
		BufferModel(pair.first, pair.second);
		delete(pair.second);
	}
	m_modelsToLoad.clear();
	m_modelsToLoadFromSource.clear();
	m_staticModelsToLoad.clear();
}

void GraphicDevice::BufferLightsToGPU_GD()
{
	if (m_directionalLightPtr)
	{
		m_dirLightDirection = vec3(m_directionalLightPtr[0], m_directionalLightPtr[1], m_directionalLightPtr[2]);
		vec3 intens = vec3(m_directionalLightPtr[3], m_directionalLightPtr[4], m_directionalLightPtr[5]);
		vec3 color = vec3(m_directionalLightPtr[6], m_directionalLightPtr[7], m_directionalLightPtr[8]);

		m_forwardShader.SetUniVariable("dirlightDirection", vector3, &m_dirLightDirection);
		m_forwardShader.SetUniVariable("dirlightIntensity", vector3, &intens);
		m_forwardShader.SetUniVariable("dirlightColor", vector3, &color);

		m_riverShader.SetUniVariable("dirlightDirection", vector3, &m_dirLightDirection);
		m_riverShader.SetUniVariable("dirlightIntensity", vector3, &intens);
		m_riverShader.SetUniVariable("dirlightColor", vector3, &color);

		m_riverCornerShader.SetUniVariable("dirlightDirection", vector3, &m_dirLightDirection);
		m_riverCornerShader.SetUniVariable("dirlightIntensity", vector3, &intens);
		m_riverCornerShader.SetUniVariable("dirlightColor", vector3, &color);
		
		m_animationShader.SetUniVariable("dirlightDirection", vector3, &m_dirLightDirection);
		m_animationShader.SetUniVariable("dirlightIntensity", vector3, &intens);
		m_animationShader.SetUniVariable("dirlightColor", vector3, &color);
	}
	else
	{
		m_forwardShader.SetUniVariable("dirlightIntensity", vector3, &m_lightDefaults[0]);
		m_forwardShader.SetUniVariable("dirlightColor", vector3, &m_lightDefaults[0]);
		m_riverShader.SetUniVariable("dirlightIntensity", vector3, &m_lightDefaults[0]);
		m_riverShader.SetUniVariable("dirlightColor", vector3, &m_lightDefaults[0]);
		m_riverCornerShader.SetUniVariable("dirlightIntensity", vector3, &m_lightDefaults[0]);
		m_riverCornerShader.SetUniVariable("dirlightColor", vector3, &m_lightDefaults[0]);
		m_animationShader.SetUniVariable("dirlightColor", vector3, &m_lightDefaults[0]);
	}

	// ------------Pointlights------------
	if (m_pointlightsPtr)
	{
		if (m_nrOfLightsToBuffer == 0)
		{
			for (int i = 0; i < 1; i++)
			{
				std::stringstream ss;
				ss << "pointlights[" << i << "].Position";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[0]);
				m_riverShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[0]);
				m_riverCornerShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[0]);
				m_animationShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[0]);
				ss.str(std::string());

				ss << "pointlights[" << i << "].Intensity";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[3]);
				m_riverShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[3]);
				m_riverCornerShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[3]);
				m_animationShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[3]);
				ss.str(std::string());

				ss << "pointlights[" << i << "].Color";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[6]);
				m_riverShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[6]);
				m_riverCornerShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[6]);
				m_animationShader.SetUniVariable(ss.str().c_str(), vector3, &m_lightDefaults[6]);
				ss.str(std::string());

				ss << "pointlights[" << i << "].Range";
				m_forwardShader.SetUniVariable(ss.str().c_str(), glfloat, &m_lightDefaults[9]);
				m_riverShader.SetUniVariable(ss.str().c_str(), glfloat, &m_lightDefaults[9]);
				m_riverCornerShader.SetUniVariable(ss.str().c_str(), glfloat, &m_lightDefaults[9]);
				m_animationShader.SetUniVariable(ss.str().c_str(), glfloat, &m_lightDefaults[9]);
				ss.str(std::string());
			}
		}
		else
		{
			for (int i = 0; i < 1; i++)
			{
				std::stringstream ss;
				ss << "pointlights[" << i << "].Position";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][0]);		
				m_riverShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][0]);		
				m_riverCornerShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][0]);
				m_animationShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][0]);
				ss.str(std::string());

				ss << "pointlights[" << i << "].Intensity";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][3]);		
				m_riverShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][3]);		
				m_riverCornerShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][3]);
				m_animationShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][3]);
				ss.str(std::string());

				ss << "pointlights[" << i << "].Color";
				m_forwardShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][6]);		
				m_riverShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][6]);		
				m_riverCornerShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][6]);
				m_animationShader.SetUniVariable(ss.str().c_str(), vector3, &m_pointlightsPtr[i][6]);
				ss.str(std::string());

				ss << "pointlights[" << i << "].Range";
				m_forwardShader.SetUniVariable(ss.str().c_str(), glfloat, &m_pointlightsPtr[i][9]);		
				m_riverShader.SetUniVariable(ss.str().c_str(), glfloat, &m_pointlightsPtr[i][9]);		
				m_riverCornerShader.SetUniVariable(ss.str().c_str(), glfloat, &m_pointlightsPtr[i][9]);
				m_animationShader.SetUniVariable(ss.str().c_str(), glfloat, &m_pointlightsPtr[i][9]);
				ss.str(std::string());
			}
		}
		delete[] m_pointlightsPtr;
		m_pointlightsPtr = 0;
	}
}

struct sort_depth
{
	inline bool operator() (const Model& a, const Model& b)
	{
		return (*a.modelMatrix)[3][2] < (*b.modelMatrix)[3][2];
	}
};

void GraphicDevice::SortModelsBasedOnDepth(std::vector<Model>* models)
{
	std::sort(models->begin(), models->end(), sort_depth());
}

void GraphicDevice::AddParticleEffect(std::string _name, const vec3 _pos, const vec3 _vel, int _nParticles, float _lifeTime, vec3 _scale, float _spriteSize, std::string _texture, vec3 _color, int &_id)
{
	ParticleSystemToLoad tmpSystem;
	tmpSystem.Name = _name;
	tmpSystem.Pos = _pos;
	tmpSystem.Vel = _vel;
	tmpSystem.NrOfParticles = _nParticles;
	tmpSystem.LifeTime = _lifeTime;
	tmpSystem.Scale = _scale;
	tmpSystem.SpriteSize = _spriteSize;
	tmpSystem.TextureName = _texture;
	tmpSystem.Color = _color;
	tmpSystem.Id = m_particleID;

	m_particleSystemsToLoad.push_back(tmpSystem);

	_id = m_particleID;
	m_particleID++;
}

void GraphicDevice::RemoveParticleEffect(int _id)
{
	m_particleEffects[_id]->EnterEndPhase();
	m_particlesIdToRemove.push_back(_id);
}

void GraphicDevice::SetParticleAcceleration(int _id, float x, float y, float z)
{
	m_particleEffects[_id]->SetAccel(vec3(x, y, z));
}

void GraphicDevice::BufferParticleSystems()
{
	// ParticleSystems to remove
	for (int i = m_particlesIdToRemove.size() - 1; i >= 0; i--)
	{
		if (m_particleEffects[m_particlesIdToRemove[i]]->ReadyToBeDeleted())
		{
			delete(m_particleEffects[m_particlesIdToRemove[i]]);
			m_particleEffects.erase(m_particlesIdToRemove[i]);
			m_particlesIdToRemove.erase(m_particlesIdToRemove.begin() + i);
		}
	}

	// ParticleSystems to add
	for (int i = 0; i < m_particleSystemsToLoad.size(); i++)
	{
		if (m_particleSystemsToLoad[i].Name == "fire")
		{
			m_particleEffects.insert(std::pair<int, ParticleEffect*>(m_particleSystemsToLoad[i].Id, new Fire(
				m_particleSystemsToLoad[i].Pos,
				m_particleSystemsToLoad[i].Vel,
				m_particleSystemsToLoad[i].NrOfParticles,
				m_particleSystemsToLoad[i].LifeTime,
				m_particleSystemsToLoad[i].Scale,
				m_particleSystemsToLoad[i].SpriteSize,
				AddTexture(m_particleSystemsToLoad[i].TextureName, GL_TEXTURE1),
				m_particleSystemsToLoad[i].Color,
				m_particleShaders[m_particleSystemsToLoad[i].Name])));
		}
		else if (m_particleSystemsToLoad[i].Name == "smoke")
		{
			m_particleEffects.insert(std::pair<int, ParticleEffect*>(m_particleSystemsToLoad[i].Id, new Smoke(
				m_particleSystemsToLoad[i].Pos,
				m_particleSystemsToLoad[i].Vel,
				m_particleSystemsToLoad[i].NrOfParticles,
				m_particleSystemsToLoad[i].LifeTime,
				m_particleSystemsToLoad[i].Scale,
				m_particleSystemsToLoad[i].SpriteSize,
				AddTexture(m_particleSystemsToLoad[i].TextureName, GL_TEXTURE1),
				m_particleSystemsToLoad[i].Color,
				m_particleShaders[m_particleSystemsToLoad[i].Name])));
		}
		else if (m_particleSystemsToLoad[i].Name == "waterfall")
		{
			m_particleEffects.insert(std::pair<int, ParticleEffect*>(m_particleSystemsToLoad[i].Id, new Waterfall(
				m_particleSystemsToLoad[i].Pos,
				m_particleSystemsToLoad[i].Vel,
				m_particleSystemsToLoad[i].NrOfParticles,
				m_particleSystemsToLoad[i].LifeTime,
				m_particleSystemsToLoad[i].Scale,
				m_particleSystemsToLoad[i].SpriteSize,
				AddTexture(m_particleSystemsToLoad[i].TextureName, GL_TEXTURE1),
				m_particleSystemsToLoad[i].Color,
				m_particleShaders[m_particleSystemsToLoad[i].Name])));
		}
		else if (m_particleSystemsToLoad[i].Name == "waterspawn")
		{
			m_particleEffects.insert(std::pair<int, ParticleEffect*>(m_particleSystemsToLoad[i].Id, new WaterSpawn(
				m_particleSystemsToLoad[i].Pos,
				m_particleSystemsToLoad[i].Vel,
				m_particleSystemsToLoad[i].NrOfParticles,
				m_particleSystemsToLoad[i].LifeTime,
				m_particleSystemsToLoad[i].Scale,
				m_particleSystemsToLoad[i].SpriteSize,
				AddTexture(m_particleSystemsToLoad[i].TextureName, GL_TEXTURE1),
				m_particleSystemsToLoad[i].Color,
				m_particleShaders[m_particleSystemsToLoad[i].Name])));
		}
	}
	m_particleSystemsToLoad.clear();

}

int GraphicDevice::LoadModel(ModelToLoadFromSource* _modelToLoad)
{
	int modelID = m_modelIDcounter;
	m_modelIDcounter++;

	//	Lägg till i en lista, följande
	//	std::string _dir, std::string _file, glm::mat4 *_matrixPtr, int _renderType

	ModelToLoadFromSource* modelToLoad = new ModelToLoadFromSource();
	*modelToLoad = *_modelToLoad;
	m_modelsToLoadFromSource[modelID] = modelToLoad;

	return modelID;
}

void GraphicDevice::Clear()
{
	m_modelIDcounter = 0;

	m_modelsAnimated.clear();
	m_modelsForward.clear();
	m_modelsViewspace.clear();
	m_modelsInterface.clear();
	m_modelsWater.clear();
	m_modelsWaterCorners.clear();

	for (std::map<const std::string, Buffer*>::iterator it = m_meshs.begin(); it != m_meshs.end(); ++it)
		delete(it->second);
	m_meshs.clear();

	for (std::map<const std::string, GLuint>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
		glDeleteTextures(1, &(it->second));
	m_textures.clear();

	for (int i = 0; i < m_surfaces.size(); i++)
		delete(m_surfaces[i].second);
	m_surfaces.clear();

	float **tmpPtr = new float*[1];
	BufferPointlights(0, tmpPtr);
	delete[] tmpPtr;

	if (m_pointlightsPtr)
		delete[] m_pointlightsPtr;

	m_pointlightsPtr = NULL;
	m_directionalLightPtr = NULL;

	for (std::map<int, ParticleEffect*>::iterator it = m_particleEffects.begin(); it != m_particleEffects.end(); ++it)
	{
		delete(it->second);
	}
		
	m_particleEffects.clear();
}

void GraphicDevice::CreateFullscreenQuad()
{
	float positionData[] = {
		-1.0, -1.0,
		1.0, -1.0,
		1.0, 1.0,
		1.0, 1.0,
		-1.0, 1.0,
		-1.0, -1.0
	};

	//m_fullscreen.UseProgram();

	glGenBuffers(1, &m_fullscreenQuadBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenQuadBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), positionData, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	//glEnableVertexAttribArray(0);
}

void GraphicDevice::BufferStaticModel(std::pair<int, std::vector<ModelToLoad*>> _staticModel)
{
	ObjectData obj = ModelLoader::importObject(_staticModel.second[0]->Dirs, _staticModel.second[0]->File);
	ModelExporter modelExporter;
	modelExporter.OpenFileForRead(obj.mesh.c_str());
	std::vector<float> positionData = modelExporter.ReadDataFromFile();
	std::vector<float> normalData = modelExporter.ReadDataFromFile();
	std::vector<float> tanData = modelExporter.ReadDataFromFile();
	std::vector<float> bitanData = modelExporter.ReadDataFromFile();
	std::vector<float> texCoordData = modelExporter.ReadDataFromFile();
	modelExporter.CloseFile();
	ModelToLoadFromSource* modelToLoadFromSource = new ModelToLoadFromSource();
	modelToLoadFromSource->positions.resize(_staticModel.second.size() * positionData.size());
	modelToLoadFromSource->normals.resize(_staticModel.second.size() * normalData.size());
	modelToLoadFromSource->tangents.resize(_staticModel.second.size() * tanData.size());
	modelToLoadFromSource->bitangents.resize(_staticModel.second.size() * bitanData.size());
	modelToLoadFromSource->texCoords.resize(_staticModel.second.size() * texCoordData.size());
	unsigned int vertexCount = (unsigned int)positionData.size() / 3;
	for (unsigned int i = 0; i < (unsigned int)_staticModel.second.size(); ++i)
	{
		glm::mat4 model = *_staticModel.second[i]->MatrixPtr;
		glm::mat4 transposeInverseModel = glm::transpose(glm::inverse(model));

		for (unsigned int j = 0; j < vertexCount; ++j)
		{
			glm::vec4 position = glm::vec4(positionData[j * 3], positionData[j * 3 + 1], positionData[j * 3 + 2], 1.0f);
			glm::vec4 normal = glm::vec4(normalData[j * 3], normalData[j * 3 + 1], normalData[j * 3 + 2], 0.0f);
			glm::vec4 tangent = glm::vec4(tanData[j * 3], tanData[j * 3 + 1], tanData[j * 3 + 2], 0.0f);
			glm::vec4 bitangent = glm::vec4(bitanData[j * 3], bitanData[j * 3 + 1], bitanData[j * 3 + 2], 0.0f);
			glm::vec2 texCoord = glm::vec2(texCoordData[j * 2], texCoordData[j * 2 + 1]);

			position = model * position;
			normal = transposeInverseModel * normal;
			tangent = transposeInverseModel * tangent;
			bitangent = transposeInverseModel * bitangent;

			modelToLoadFromSource->positions[i * vertexCount * 3 + j * 3 + 0] = position[0];
			modelToLoadFromSource->positions[i * vertexCount * 3 + j * 3 + 1] = position[1];
			modelToLoadFromSource->positions[i * vertexCount * 3 + j * 3 + 2] = position[2];
			modelToLoadFromSource->normals[i * vertexCount * 3 + j * 3 + 0] = normal[0];
			modelToLoadFromSource->normals[i * vertexCount * 3 + j * 3 + 1] = normal[1];
			modelToLoadFromSource->normals[i * vertexCount * 3 + j * 3 + 2] = normal[2];
			modelToLoadFromSource->tangents[i * vertexCount * 3 + j * 3 + 0] = tangent[0];
			modelToLoadFromSource->tangents[i * vertexCount * 3 + j * 3 + 1] = tangent[1];
			modelToLoadFromSource->tangents[i * vertexCount * 3 + j * 3 + 2] = tangent[2];
			modelToLoadFromSource->bitangents[i * vertexCount * 3 + j * 3 + 0] = bitangent[0];
			modelToLoadFromSource->bitangents[i * vertexCount * 3 + j * 3 + 1] = bitangent[1];
			modelToLoadFromSource->bitangents[i * vertexCount * 3 + j * 3 + 2] = bitangent[2];
			modelToLoadFromSource->texCoords[i * vertexCount * 2 + j * 2 + 0] = texCoord[0];
			modelToLoadFromSource->texCoords[i * vertexCount * 2 + j * 2 + 1] = texCoord[1];
		}
	}
	*_staticModel.second[0]->MatrixPtr = glm::mat4(1.0f);
	char nameBuffer[16];
	sprintf(nameBuffer, "Static%d", _staticModel.first);
	modelToLoadFromSource->key = std::string(nameBuffer);
	modelToLoadFromSource->diffuseTextureFilepath = obj.text;
	modelToLoadFromSource->normalTextureFilepath = obj.norm;
	modelToLoadFromSource->specularTextureFilepath = obj.spec;
	modelToLoadFromSource->MatrixPtr = _staticModel.second[0]->MatrixPtr;
	modelToLoadFromSource->RenderType = _staticModel.second[0]->RenderType;
	modelToLoadFromSource->Color = _staticModel.second[0]->Color;
	modelToLoadFromSource->CastShadow = _staticModel.second[0]->CastShadow;
	m_modelsToLoadFromSource[_staticModel.first] = modelToLoadFromSource;
	for (ModelToLoad* modelToLoad : _staticModel.second)
		delete modelToLoad;
}