#include "Buffer.h"

using namespace Renderer;

Buffer::Buffer(void)
{
	m_Type = None;
	m_Count = 0;
}

Buffer::~Buffer(void)
{
	// Clean up buffers
	if (m_Type != None)
	{
		glDeleteBuffers(m_Buffers.size(), &m_Buffers[0]);
		glDeleteVertexArrays(1, &m_VAO);
	}
}

bool Buffer::init(const BufferData* p_BufferData, GLsizei p_BufferDataSize)
{
	return init(p_BufferData, p_BufferDataSize, NULL, 0);
}

bool Buffer::init(const BufferData* p_BufferData, GLsizei p_BufferDataSize,
	const GLuint* p_Indices, GLsizei p_IndexDataSize)
{
	// Make sure there's no weird behaviour
	if (m_Type != None || p_BufferData == NULL || p_BufferDataSize == 0)
		return false;

	// Define what type of buffer that will be used
	m_Type = (p_Indices == NULL || p_IndexDataSize == 0) ? VertexBased : IndexBased;

	// Create vertex array object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create buffers
	const GLsizei bufferSize = VertexBased ? p_BufferDataSize : p_BufferDataSize + 1;
	m_Buffers.resize(bufferSize);
	glGenBuffers(bufferSize, &m_Buffers[0]);

	// Initialize every vertex buffer
	for (int i = 0; i < p_BufferDataSize; i++)
	{
		// Allocate and buffer data
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, p_BufferData[i].dataSize,
			p_BufferData[i].data, GL_STATIC_DRAW);
		// Define attribute data (for shaders)
		glEnableVertexAttribArray(p_BufferData[i].location);
		glVertexAttribPointer(p_BufferData[i].location, p_BufferData[i].componentCount,
			p_BufferData[i].type, GL_FALSE, 0, 0);
	}

	//-------FOR INSTANCING----------------------------
	glGenBuffers(1, &m_inMatVBO); //gen buffer till matrisen
	glBindBuffer(GL_ARRAY_BUFFER, m_inMatVBO);
	for (int i = 0; i < 4; i++)
	{
		glVertexAttribPointer(5 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(float)* 4 * i));
		glEnableVertexAttribArray(5 + i);
		glVertexAttribDivisor(5 + i, 1);
	}

	glGenBuffers(1, &m_normalMatVBO); //gen buffer till matrisen
	glBindBuffer(GL_ARRAY_BUFFER, m_normalMatVBO);
	for (int i = 0; i < 3; i++)
	{
		glVertexAttribPointer(9 + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (const GLvoid*)(sizeof(float)* 3 * i));
		glEnableVertexAttribArray(9 + i);
		glVertexAttribDivisor(9 + i, 1);
	}

	//glGenBuffers(1, &m_colorVBO); //gen buffer till matrisen
	//glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	//glVertexAttribPointer(15, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (const GLvoid*)(sizeof(float) * 4));
	//glEnableVertexAttribArray(15);
	//glVertexAttribDivisor(15, 1);
	//-------------------------------------------------

	// Initialize index buffer if specified
	if (m_Type == IndexBased)
	{
		// Allocate and buffer data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers.back());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_IndexDataSize, p_Indices, GL_STATIC_DRAW);
	}

	// Unbind vertex array object
	glBindVertexArray(0);

	return true;
}

void Buffer::draw(void)
{
	if (m_Count > 0)
	{
		draw(0, m_Count);
	}
}

void Buffer::draw(GLint base, GLsizei count)
{
	// Make sure there's no weird behaviour
	if (m_Type == None)
		return;

	// Bind vertex array object
	glBindVertexArray(m_VAO);

	// Draw based on based buffer type
	switch (m_Type)
	{
	case VertexBased:
		glDrawArrays(GL_TRIANGLES, base, count);
		break;
	case IndexBased:
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(sizeof(GLuint)* base));
		break;
	}

	// Unbind vertex array object
	glBindVertexArray(0);
}

void Buffer::drawInstanced(GLint base, int instances, std::vector<glm::mat4> *inMats, std::vector<glm::mat3> *normalMats, std::vector<float> *color)
{
	// Make sure there's no weird behaviour
	if (m_Type == None)
		return;

	// Bind vertex array object
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_inMatVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*instances, &(*inMats)[0], GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_normalMatVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat3)*instances, &(*normalMats)[0], GL_DYNAMIC_DRAW);

	//if (color != nullptr)
	//{ 
	//	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*instances*4, &(*color)[0], GL_STATIC_DRAW);
	//}
	// Draw based on based buffer type
	switch (m_Type)
	{
	case VertexBased:
		glDrawArraysInstanced(GL_TRIANGLES, base, m_Count, instances);
		break;
	case IndexBased:
		glDrawElementsInstanced(GL_TRIANGLES, m_Count, GL_FLOAT, (void*)(sizeof(GLuint)* base), instances);
		break;
	}

	// Unbind vertex array object
	glBindVertexArray(0);
}

void Buffer::setCount(GLsizei count)
{
	m_Count = count;
}