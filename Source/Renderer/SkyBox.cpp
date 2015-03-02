#include "SkyBox.h"
#include "ShadowMap.h"

SkyBox::SkyBox()
{ }

SkyBox::~SkyBox()
{ 
	glDeleteVertexArrays(1, &m_VAOHandle);
	glDeleteBuffers(1, &m_vboCubeVertices);
	glDeleteBuffers(1, &m_iboCubeIndices);
}

SkyBox::SkyBox(GLuint _texHandle, float _camFarPlane)
{
	m_textureHandle = _texHandle;
	BindBuffers(_camFarPlane*0.55);
}

void SkyBox::BindBuffers(float _far)
{
	// cube vertices for vertex buffer object
    static const GLfloat cubeVertices[] = {
        -1.0f*_far, -1.0f*_far,  1.0f*_far,
        1.0f*_far, -1.0f*_far,  1.0f*_far,
        -1.0f*_far,  1.0f*_far,  1.0f*_far,
        1.0f*_far,  1.0f*_far,  1.0f*_far,
        -1.0f*_far, -1.0f*_far, -1.0f*_far,
        1.0f*_far, -1.0f*_far, -1.0f*_far,
        -1.0f*_far,  1.0f*_far, -1.0f*_far,
        1.0f*_far,  1.0f*_far, -1.0f*_far,
    };
    
    static const GLushort cubeIndices[] = {
        0, 1, 2, 3,
        1, 5, 3, 7,
        5, 4, 7, 6, 
        7, 6, 3, 2,
        2, 6, 0, 4,
        0, 4, 1, 5,
    };

	glGenVertexArrays(1, &m_VAOHandle);
	glBindVertexArray(m_VAOHandle);

	glGenBuffers(1, &m_vboCubeVertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboCubeVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);


	glGenBuffers(1, &m_iboCubeIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboCubeIndices);
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
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureHandle);
	glBindVertexArray(m_VAOHandle);
	glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_SHORT, 0);
}