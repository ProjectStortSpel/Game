/*Author: Anders*/
#ifndef SIMPLETEXT_H
#define SIMPLETEXT_H

#include "Shader.h"
#include "stdafx.h"

#ifndef __ANDROID__
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

		int GetArraySize(){ return simpleText.size(); }

		void Disco() { m_disco = !m_disco; };

	private:
		float m_clock;
		float m_update;

		bool m_disco;

		GLuint m_textImage;

		Shader m_simpleTextShader;

		std::vector<int> simpleText;
		int simpleTextX, simpleTextY;
		GLuint simpleTextBuffer;
	};
}
#endif
#endif