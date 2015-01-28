#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <SDL/SDL_ttf.h>
#include <GLEW/glew.h>
#include <string>
#include <vector>

class DECLSPEC TextRenderer
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