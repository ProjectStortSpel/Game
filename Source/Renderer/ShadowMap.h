#ifndef SHADOWMAP_H
#define SHADOWMAP_H
#include "stdafx.h"

using glm::vec3;
using glm::mat4;

class ShadowMap
{
private:
	GLuint m_shadowFBO; //handle to framebuffer
	GLsizei m_resolution;
	GLuint m_depthTex;

	vec3 m_lightPosition;
	vec3 m_targetDirection;
	mat4 m_viewMatrix;
	mat4 m_projectionMatrix;
	mat4 m_biasMatrix;

public:
	ShadowMap(vec3 lightPos, vec3 target, int res);
	ShadowMap();
	~ShadowMap();

	void CreateShadowMapTexture(GLuint _textureUnit);
	void ChangeResolution(int res);

	int GetResolution(){ return m_resolution; }
	
	GLuint GetShadowFBOHandle()
		{ return m_shadowFBO; }
	GLuint GetDepthTexHandle(){ return m_depthTex; }
	mat4* GetViewMatrix(){ return &m_viewMatrix; }
	mat4* GetProjectionMatrix(){ return &m_projectionMatrix; }
	mat4* GetBiasMatrix(){ return &m_biasMatrix; }
	void SetLightPos(vec3 pos);
};
#endif 

