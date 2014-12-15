#ifndef SHADOWMAP_H_
#define SHADOWMAP_H_
#include "stdafx.h"

using glm::vec3;
using glm::mat4;

class ShadowMap
{
private:
	GLuint mShadowFBO; //handle to framebuffer
	GLsizei mResolution;
	GLuint mDepthTex;

	vec3 mLightPosition;
	vec3 mTargetDirection;
	mat4 mViewMatrix;
	mat4 mProjectionMatrix;

public:
	ShadowMap(vec3 lightPos, vec3 target, int res);
	ShadowMap();
	~ShadowMap();

	void CreateShadowMapTexture(int i);
	void ChangeResolution(int res);
	
	GLuint GetShadowFBOHandle()
		{ return mShadowFBO; }
	GLuint GetDepthTexHandle()
		{ return mDepthTex; }
	mat4 GetViewMatrix()
		{ return mViewMatrix; }
	mat4 GetProjectionMatrix()
		{ return mProjectionMatrix; }
	mat4 GetBiasMatrix();
	void SetLightPos(vec3 pos);
};
#endif 

