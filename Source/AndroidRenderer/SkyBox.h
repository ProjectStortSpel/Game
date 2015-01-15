#ifndef SKYBOX_H_
#define SKYBOX_H_
#include "stdafx.h"
#include "Camera.h"

using glm::mat4;

class SkyBox
{
private:
	GLuint m_textureHandle;
	GLuint m_VAOHandle;

public:
	SkyBox(GLuint _texHandle, float _camFarPlane);
	SkyBox();
	~SkyBox();

	void BindBuffers(float _camFarPlane);
	void Draw(GLuint _shaderProgHandle, Camera *_cam);
};

#endif