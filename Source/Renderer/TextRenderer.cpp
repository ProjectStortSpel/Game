#include "TextRenderer.h"
#include <assert.h>

TextRenderer::TextRenderer()
{
	m_initialized = false;
}

TextRenderer::~TextRenderer()
{
	for (TTF_Font* font : m_fonts)
		TTF_CloseFont(font);
	
	if (m_initialized)
		TTF_Quit();
}

bool TextRenderer::Init()
{
	assert(!m_initialized);
	
	if (TTF_Init() < 0)
		return false;
	
	m_initialized = true;
	
	return true;
}

int TextRenderer::AddFont(const std::string& filepath, int size)
{
	assert(m_initialized && size > 0);
	
	TTF_Font* font = TTF_OpenFont(filepath.c_str(), size);
	if (!font)
		return -1;
	m_fonts.push_back(font);
	return (int)m_fonts.size() - 1;
}

SDL_Surface* TextRenderer::CreateTextSurface(const std::string& textString, int fontIndex, SDL_Color color)
{
	assert(m_initialized && fontIndex >= 0 && fontIndex < (int)m_fonts.size());
	
	return TTF_RenderText_Blended(m_fonts[fontIndex], textString.c_str(), color);
}

SDL_Surface* TextRenderer::CreateWrappedTextSurface(const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength)
{
	assert(m_initialized && fontIndex >= 0 && fontIndex < (int)m_fonts.size());
	
	return TTF_RenderText_Blended_Wrapped(m_fonts[fontIndex], textString.c_str(), color, wrapLength);
}