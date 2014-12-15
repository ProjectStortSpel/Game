#include "ShadowMap.h"


ShadowMap::ShadowMap(vec3 lightPos, vec3 target, int res)
{
	mLightPosition = lightPos;
	mTargetDirection = target;

	mResolution = res;
	mProjectionMatrix = glm::perspective(140.0f, (float)res/(float)res, 1.0f, 300.0f);
	mViewMatrix = glm::lookAt(mLightPosition, mTargetDirection, vec3(0.0f, 1.0f, 0.0f));

}

ShadowMap::ShadowMap()
{
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::CreateShadowMapTexture(int i)
{
	GLfloat border[] = { 1.0, 0.0, 0.0, 0.0 };

	//The shadow maptexture  
	glGenTextures(1, &mDepthTex); 
	glBindTexture(GL_TEXTURE_2D, mDepthTex); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mResolution, mResolution, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); 
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	//Assign the shadow map to texture channel 1 
	glActiveTexture(GL_TEXTURE5+i);
	glBindTexture(GL_TEXTURE_2D, mDepthTex);

	//Create and set up the FBO 
	glGenFramebuffers(1, &mShadowFBO); 
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowFBO); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTex, 0);
	GLenum drawBuffers[] = { GL_NONE }; 
	glDrawBuffers(1, drawBuffers);
	
	// Revert to the default framebuffer for now 
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void ShadowMap::SetLightPos(vec3 pos)
{ 
	mViewMatrix = glm::lookAt(pos, pos+vec3(0.0f, -1.0f, -0.001f), vec3(0.0, 1.0, 0.0));
}

void ShadowMap::ChangeResolution(int res)
{
	mResolution = res;
	glBindTexture(GL_TEXTURE_2D, mDepthTex); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mResolution, mResolution, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0); 
}

mat4 ShadowMap::GetBiasMatrix()
{
	return  mat4( 0.5, 0.0, 0.0, 0.0,
                  0.0, 0.5, 0.0, 0.0,
                  0.0, 0.0, 0.5, 0.0,
                  0.5, 0.5, 0.5, 1.0);
}