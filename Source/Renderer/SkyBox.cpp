#include "SkyBox.h"
#include "ShadowMap.h"

SkyBox::SkyBox()
{ }

SkyBox::~SkyBox()
{ }

SkyBox::SkyBox(GLuint _texHandle, float _camFarPlane)
{
	m_textureHandle = _texHandle;
	BindBuffers(_camFarPlane*0.55);
}

void SkyBox::BindBuffers(float _far)
{
	// cube vertices for vertex buffer object
	GLfloat cubeVertices[] = {
	  static_cast<GLfloat>(-1.0*_far),  static_cast<GLfloat>(1.0*_far),  static_cast<GLfloat>(1.0*_far),
	  static_cast<GLfloat>(-1.0*_far), static_cast<GLfloat>(-1.0*_far),  static_cast<GLfloat>(1.0*_far),
	   static_cast<GLfloat>(1.0*_far), static_cast<GLfloat>(-1.0*_far),  static_cast<GLfloat>(1.0*_far),
	   static_cast<GLfloat>(1.0*_far),  static_cast<GLfloat>(1.0*_far),  static_cast<GLfloat>(1.0*_far),
	  static_cast<GLfloat>(-1.0*_far),  static_cast<GLfloat>(1.0*_far), static_cast<GLfloat>(-1.0*_far),
	  static_cast<GLfloat>(-1.0*_far), static_cast<GLfloat>(-1.0*_far), static_cast<GLfloat>(-1.0*_far),
	   static_cast<GLfloat>(1.0*_far), static_cast<GLfloat>(-1.0*_far), static_cast<GLfloat>(-1.0*_far),
	   static_cast<GLfloat>(1.0*_far),  static_cast<GLfloat>(1.0*_far), static_cast<GLfloat>(-1.0*_far),
	};

	// cube indices for index buffer object
	GLuint cubeIndices[] = {
	  3, 2, 1, 0,
	  7, 6, 2, 3,
	  4, 5, 6, 7,
	  0, 1, 5, 4,
	  4, 7, 3, 0,
	 1, 2, 6, 5,
	};

	glGenVertexArrays(1, &m_VAOHandle);
	glBindVertexArray(m_VAOHandle);

	GLuint vboCubeVertices;
	glGenBuffers(1, &vboCubeVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboCubeVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);


	GLuint iboCubeIndices;
	glGenBuffers(1, &iboCubeIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCubeIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void SkyBox::Draw(GLuint _shaderProgHandle, Camera *_cam)
{
	GLuint location = glGetUniformLocation(_shaderProgHandle, "MVP");

	mat4 MVP = *_cam->GetProjMatrix() * (*_cam->GetViewMatrix()) * glm::translate(mat4(1.0f), *_cam->GetPos());

	glUniformMatrix4fv(location, 1, GL_FALSE, &MVP[0][0]);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);
	glBindVertexArray(m_VAOHandle);
	glDrawElements( GL_QUADS, 24, GL_UNSIGNED_INT, 0);
}