#include <SDL/SDL.h>
#include "ECSL/ECSL.h"

#include "Renderer/GraphicDevice.h"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	Renderer::GraphicDevice* gd = new Renderer::GraphicDevice();

	gd->Init();

	while (true)
		gd->Render();

	SDL_Quit();
	return 0;
}