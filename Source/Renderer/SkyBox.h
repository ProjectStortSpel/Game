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
	GLuint m_vboCubeVertices;
	GLuint m_iboCubeIndices;
	float m_rotAngle;

public:
	SkyBox(GLuint _texHandle, float _camFarPlane, float _rotationSpeed);
	SkyBox();
	~SkyBox();

	void BindBuffers(float _camFarPlane);
	void Draw(GLuint _shaderProgHandle, Camera *_cam);
};

#endif