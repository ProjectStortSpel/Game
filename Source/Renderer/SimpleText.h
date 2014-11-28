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

		bool Init(GLuint _textimage, int _clientWidth, int _clientHeight);

		void RenderText(float _dt);

		bool RenderSimpleText(std::string _text, int x, int y);
		void SetSimpleTextColor(glm::vec4 _color);

	private:
		float m_clock;
		float m_update;

		GLuint m_textImage;

		Shader m_simpleTextShader;

		int simpleText[4608];
		int simpleTextX, simpleTextY;
		GLuint simpleTextBuffer;
	};
}
#endif