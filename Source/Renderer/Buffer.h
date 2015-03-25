/*Author: Anders*/
#ifndef BUFFER_H
#define BUFFER_H

#include "stdafx.h"

namespace Renderer
{
	struct BufferData
	{
		GLuint location;
		GLint componentCount;
		GLenum type;

		const GLvoid* data;
		GLsizeiptr dataSize;
	};

	class Buffer
	{
	public:
		Buffer(void);
		~Buffer(void);

		// Returns true on success
		bool init(const BufferData* p_BufferData, GLsizei p_BufferDataSize);
		bool init(const BufferData* p_BufferData, GLsizei p_BufferDataSize,
			const GLuint* p_Indices, GLsizei p_IndexDataSize);
		bool initNotInstanced(const BufferData* p_BufferData, GLsizei p_BufferDataSize,
			const GLuint* p_Indices, GLsizei p_IndexDataSize);

		void draw(void);
		void draw(GLint base, GLsizei count);
		void drawInstanced(GLint base, int instances, std::vector<glm::mat4> *inMats, std::vector<glm::mat3> *normalMats, float *color = nullptr);

		void setCount(GLsizei count);
		int getCount() { return (int)m_Count; }

		GLuint getVAO() { return m_VAO; }

	private:
		enum Type
		{
			None,
			VertexBased,
			IndexBased
		} m_Type;

		GLuint m_VAO;
		std::vector<GLuint> m_Buffers;
		GLuint m_inMatVBO, m_normalMatVBO, m_colorVBO;
		GLsizei m_Count;
	};
}

#endif