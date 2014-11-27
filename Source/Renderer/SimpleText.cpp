#include "SimpleText.h"

using namespace Renderer;

SimpleText::SimpleText()
{
}

SimpleText::~SimpleText()
{
}

bool SimpleText::Init(GLuint* _outputimage, GLuint _textimage, int _clientWidth, int _clientHeight)
{
	// Set output image
	m_outputImage = _outputimage;
	m_textImage = _textimage;

	// Shader Shader
	m_simpleTextShader.InitShaderProgram();
	m_simpleTextShader.AddShader("content/shaders/SimpleText.glsl", GL_COMPUTE_SHADER);
	m_simpleTextShader.FinalizeShaderProgram();

	// Buffer init
	m_simpleTextShader.UseProgram();
	SetSimpleTextColor(vec4(1, 1, 1, 1));

	//m_textImage = TextureLoader::LoadTexture("Content/Textures/SimpleText.png", 5);

	glGenTextures(1, m_outputImage);

	glBindTexture(GL_TEXTURE_2D, *m_outputImage);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, _clientWidth, _clientHeight);

	simpleTextX = _clientWidth / 8;
	simpleTextY = _clientHeight / 16;
	glGenBuffers(1, &simpleTextBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, simpleTextBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int)* 4608, &simpleText, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, simpleTextBuffer);

	return true;
}

void SimpleText::RenderText()
{
	glBindBuffer(GL_ARRAY_BUFFER, simpleTextBuffer);
	GLvoid* p = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &simpleText, sizeof(int)* 4608);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	m_simpleTextShader.UseProgram();
	// Run program

	// Bind buffers
	glBindImageTexture(5, m_textImage, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glBindImageTexture(6, *m_outputImage, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	// run program
	glDispatchCompute(128, 36, 1); // 1/16 = 0.0625

	for (int i = 0; i < 4608; i++)
	{
		simpleText[i] = 32;
	}
}

bool SimpleText::RenderSimpleText(std::string _text, int _x, int _y)
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

	return true;
}

void SimpleText::SetSimpleTextColor(vec4 _color)
{
	m_simpleTextShader.UseProgram();
	m_simpleTextShader.SetUniVariable("text_color", vector4, &_color);
}