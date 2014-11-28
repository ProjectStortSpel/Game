#ifndef SIMPLETEXT_H
#define SIMPLETEXT_H

#include "Shader.h"
#include "stdafx.h"

namespace Renderer
{
	class DECLSPEC SimpleText
	{
	public:
		SimpleText();
		~SimpleText();

		bool Init(GLuint* _outputimage, GLuint _textimage, int _clientWidth, int _clientHeight);

		void RenderText();

		bool RenderSimpleText(std::string _text, int x, int y);
		void SetSimpleTextColor(vec4 _color);

	private:
		GLuint* m_outputImage;
		GLuint m_textImage;

		Shader m_simpleTextShader;

		int simpleText[4608];
		int simpleTextX, simpleTextY;
		GLuint simpleTextBuffer;
	};
}
#endif