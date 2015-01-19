#ifndef SKYBOX_H_
#define SKYBOX_H_
#include "stdafx.h"
#include "Camera.h"

using glm::mat4;

class SkyBox
{
private:
	GLuint m_textureHandle;
	GLuint m_attribLoc;

	GLuint vboCubeVertices;
	GLuint iboCubeIndices;

public:
	SkyBox(GLuint _texHandle, float _camFarPlane, GLuint loc);
	SkyBox();
	~SkyBox();

	void BindBuffers(float _camFarPlane);
	void Draw(GLuint _shaderProgHandle, Camera *_cam);
};

#endif