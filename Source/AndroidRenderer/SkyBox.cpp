#include "SkyBox.h"

SkyBox::SkyBox()
{ }

SkyBox::~SkyBox()
{
	glDeleteBuffers(1, &vboCubeVertices);
	glDeleteBuffers(1, &iboCubeIndices);
	glDeleteTextures(1, &m_textureHandle);
}

SkyBox::SkyBox(GLuint _texHandle, float _camFarPlane, GLuint loc)
{
	m_textureHandle = _texHandle;
	m_attribLoc = loc;
	BindBuffers(_camFarPlane*0.55f);
}

void SkyBox::BindBuffers(float _far)
{
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

	glGenBuffers(1, &vboCubeVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboCubeVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &iboCubeIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCubeIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

}

void SkyBox::Draw(GLuint _shaderProgHandle, Camera *_cam)
{
	GLuint location = glGetUniformLocation(_shaderProgHandle, "MVP");

	mat4 MVP = (*_cam->GetProjMatrix()) * (*_cam->GetViewMatrix()) * glm::translate(glm::mat4(1.0f), (*_cam->GetPos()));

	glUniformMatrix4fv(location, 1, GL_FALSE, &MVP[0][0]);
	
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, m_textureHandle);

	glBindBuffer(GL_ARRAY_BUFFER, vboCubeVertices);
	glVertexAttribPointer(m_attribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(m_attribLoc);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCubeIndices);
	//glVertexAttribPointer(m_attribLoc, m_BufferData[i].componentCount, m_BufferData[i].type, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(m_BufferData[i].location);

	glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_SHORT, 0);
}