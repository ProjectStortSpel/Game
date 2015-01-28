#include "ShadowMap.h"


ShadowMap::ShadowMap(vec3 lightPos, vec3 target, int res)
{
	m_lightPosition = lightPos;
	m_targetDirection = target;

	m_resolution = res;
	m_projectionMatrix = glm::ortho(-10.0, 10.0, -10.0, 10.0, 3.0, 18.0); 
	//glm::perspective(45.0f, (float)res / (float)res, 1.0f, 50.0f);
	m_viewMatrix = glm::lookAt(m_lightPosition, m_targetDirection, vec3(0.0f, 1.0f, 0.0f));

	float val = 0.5f;
	m_biasMatrix = mat4(val, 0.0f, 0.0f, 0.0f,
						0.0f, val, 0.0f, 0.0f,
						0.0f, 0.0f, val, 0.0f,
						val, val, val, 1.0f);
}

ShadowMap::ShadowMap()
{
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::CreateShadowMapTexture(GLuint _textureUnit)
{
	//The shadow maptexture  
	glGenTextures(1, &m_depthTex); 
	glActiveTexture(_textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_depthTex);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_resolution, m_resolution, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_resolution, m_resolution, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

    GLint oldFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
    
	//Create and set up the FBO 
	glGenFramebuffers(1, &m_shadowFBO); 
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{ SDL_Log("Error: FrameBufferObject is not complete!"); }
	
	// Revert to the default framebuffer for now 
	glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
}

void ShadowMap::UpdateViewMatrix(vec3 lightPos, vec3 target)
{ 
	m_viewMatrix = glm::lookAt(lightPos, target, vec3(0.0f, 1.0f, 0.0f));
}

void ShadowMap::ChangeResolution(int res)
{
	m_resolution = res;
	glBindTexture(GL_TEXTURE_2D, m_depthTex); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_resolution, m_resolution, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0); 
}
