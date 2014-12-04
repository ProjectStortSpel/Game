#include "SimpleText.h"

using namespace Renderer;
using namespace glm;

SimpleText::SimpleText()
{
	m_update = 0.5f;
	m_clock = 0;
}

SimpleText::~SimpleText()
{
}

bool SimpleText::Init(GLuint _textimage, int _clientWidth, int _clientHeight)
{
	// Set output image
	m_textImage = _textimage;

	// Shader Shader
	m_simpleTextShader.InitShaderProgram();
	m_simpleTextShader.AddShader("content/shaders/SimpleText.glsl", GL_COMPUTE_SHADER);
	m_simpleTextShader.FinalizeShaderProgram();

	// Buffer init
	m_simpleTextShader.UseProgram();
	SetSimpleTextColor(vec4(1, 1, 1, 1));

	simpleTextX = _clientWidth / 8;
	simpleTextY = _clientHeight / 16;

	for (int i = 0; i < simpleTextX*simpleTextY; i++)
	{
		simpleText.push_back(32);
	}

	glGenBuffers(1, &simpleTextBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, simpleTextBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int)* simpleText.size(), &simpleText[0], GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, simpleTextBuffer);

	return true;
}

void SimpleText::RenderText(float _dt)
{
	m_clock += _dt;

	if (m_clock > 10)
	{
		glBindBuffer(GL_ARRAY_BUFFER, simpleTextBuffer);
		GLvoid* p = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(p, &simpleText[0], sizeof(int)* simpleText.size());
		glUnmapBuffer(GL_ARRAY_BUFFER);

		m_clock = 0;
	}

	m_simpleTextShader.UseProgram();
	// Run program

	// Bind buffers
	glBindImageTexture(1, m_textImage, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	//glBindImageTexture(5, *m_outputImage, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	if (m_clock > m_update)
	{
		for (int i = 0; i < simpleText.size(); i++)
		{
			simpleText[i] = 32;
		}
		m_clock = 10;
	}

	// run program
	glDispatchCompute(simpleTextX, simpleTextY, 1); // 1/16 = 0.0625
}

bool SimpleText::RenderSimpleText(std::string _text, int _x, int _y)
{
	if (m_clock > 9)
	{
		int xoffset = 0;
		int yoffset = 0;

		for (int i = 0; i < _text.size(); i++)
		{
			int thischar = (int)_text[i];
			if (thischar == 10)
			{
				xoffset = 0;
				yoffset++;
			}
			else
			{
				int oob = _x + xoffset % (simpleTextX + 1);
				if (oob == simpleTextX)
				{
					xoffset = 0;
					yoffset++;
				}
				int xy = _x + xoffset + (_y + yoffset) * simpleTextX;
				if (xy > simpleTextX*simpleTextY)
					return false;
				else
					simpleText[xy] = thischar;

				xoffset++;
			}
		}

	}
	return true;
}

void SimpleText::SetSimpleTextColor(vec4 _color)
{
	m_simpleTextShader.UseProgram();
	m_simpleTextShader.SetUniVariable("text_color", vector4, &_color);
}