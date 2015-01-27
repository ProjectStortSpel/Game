#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <SDL/SDL_ttf.h>
#ifndef __IOS__
#include <GLES2/gl2.h>
#endif
#include <SDL/SDL_opengles2.h>
#include <string>
#include <vector>

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();
	
	bool Init();
	
	int AddFont(const std::string& filepath, int size);
	
	SDL_Surface* CreateTextSurface(const std::string& textString, int fontIndex, SDL_Color color);
	SDL_Surface* CreateWrappedTextSurface(const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength);
	
private:
	bool m_initialized;
	std::vector<TTF_Font*> m_fonts;
};

#endif