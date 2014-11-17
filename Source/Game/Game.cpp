#include <SDL/SDL.h>
//#include <Renderer/GraphicDevice.h>


int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	/*Renderer::GraphicDevice* gdevice = new Renderer::GraphicDevice();
	gdevice->Init();

	while (true)
	{
		
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_WINDOWEVENT:
				gdevice->PollEvent(e);
			default:
				break;
			}
		}
		gdevice->Render();
	}*/

	SDL_Quit();
	return 0;
}