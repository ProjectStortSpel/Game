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
		bool init(const BufferData* p_BufferData, GLsizei p_BufferDataSize, GLuint program);
		bool init(const BufferData* p_BufferData, GLsizei p_BufferDataSize,
			const GLuint* p_Indices, GLsizei p_IndexDataSize, GLuint program);

		void draw(void);
		void draw(GLint base, GLsizei count);

		void setCount(GLsizei count);

	private:
		enum Type
		{
			None,
			VertexBased,
			IndexBased
		} m_Type;

		std::vector<GLuint> m_Buffers;
		GLuint m_inMatVBO, m_normalMatVBO;
		GLsizei m_Count;
		std::vector<BufferData> m_BufferData;
	};
}

#endif