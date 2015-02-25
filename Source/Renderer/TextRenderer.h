#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <SDL/SDL_ttf.h>
#include <GLEW/glew.h>
#include <string>
#include <vector>

class DECLSPEC TextRenderer
{
public:
	static bool Init();

	static void Clean();
	
	static int AddFont(const std::string& filepath, int size);
	
	static SDL_Surface* CreateTextSurface(const std::string& textString, int fontIndex, SDL_Color color);
	static SDL_Surface* CreateWrappedTextSurface(const std::string& textString, int fontIndex, SDL_Color color, unsigned int wrapLength);
	
private:
	static bool m_initialized;
	static std::vector<TTF_Font*> m_fonts;
};

#endif