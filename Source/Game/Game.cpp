#include <SDL/SDL.h>
#include "ECSL/ECSL.h"
#include "Input/InputWrapper.h"
#include "Renderer/GraphicDevice.h"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	Renderer::GraphicDevice* gd = new Renderer::GraphicDevice();
	Input::InputWrapper INPUT = Input::InputWrapper::GetInstance();
	gd->Init();

	bool lol = true;
	while (lol)
	{
		gd->Render();
		INPUT.Update();
		
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			INPUT.PollEvent(e);
		}

		if (INPUT.GetKeyboard()->GetKeyState(SDL_SCANCODE_SPACE) == Input::InputState::PRESSED)
			lol = false;
	}
		
	delete gd;
	SDL_Quit();
	return 0;
}