#include "ShadowMap.h"


ShadowMap::ShadowMap(vec3 lightPos, vec3 target, int res)
{
	m_lightPosition = lightPos;
	m_targetDirection = target;

	m_resolution = res;
	m_projectionMatrix = glm::ortho(-10.0, 10.0, -10.0, 10.0, 1.5, 18.0); 
	//glm::perspective(45.0f, (float)res / (float)res, 1.0f, 50.0f);
	m_viewMatrix = glm::lookAt(m_lightPosition, m_targetDirection, vec3(0.0f, 1.0f, 0.0f));
	float val = 0.5f;
	m_biasMatrix = mat4(val, 0.0, 0.0, 0.0,
						0.0, val, 0.0, 0.0,
						0.0, 0.0, val, 0.0,
						val, val, val, 1.0);
}

ShadowMap::ShadowMap()
{
}

ShadowMap::~ShadowMap()
{
	glDeleteFramebuffers(1, &m_shadowFBO);
}

void ShadowMap::CreateShadowMapTexture(GLuint _textureUnit)
{
	GLfloat border[] = { 1.0, 1.0, 1.0, 1.0 };

	//The shadow maptexture  
	glGenTextures(1, &m_depthTex); 
	glBindTexture(GL_TEXTURE_2D, m_depthTex); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_resolution, m_resolution, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); 
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	//Assign the shadow map to texture channel 1 
	glActiveTexture(_textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_depthTex);

	//Create and set up the FBO 
	glGenFramebuffers(1, &m_shadowFBO); 
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTex, 0);
	GLenum drawBuffers[] = { GL_NONE }; 
	glDrawBuffers(1, drawBuffers);
	
	// Revert to the default framebuffer for now 
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
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

void ShadowMap::SetBounds(float _width, float _height)
{
	m_width = _width;
	m_height = _height;
	m_projectionMatrix = glm::ortho(-_width, _width, -_height, _height, 1.5f, 10.f + std::max(_width, _height));
}
