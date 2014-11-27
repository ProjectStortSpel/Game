#include <SDL/SDL.h>

#include "Timer.h"

#include "ECSL/ECSL.h"
#include "Input/InputWrapper.h"
#include "Renderer/GraphicDevice.h"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	Timer timer;

	Renderer::GraphicDevice* gd = new Renderer::GraphicDevice();
	Input::InputWrapper INPUT = Input::InputWrapper::GetInstance();
	gd->Init();

	bool lol = true;
	while (lol)
	{
		// DT COUNTER
		float dt = timer.ElapsedTimeInSeconds();
		timer.Reset();

		INPUT.Update();
		gd->Update(dt);

		gd->RenderSimpleText("This text render from GAME! \nThe x and y values in the function isn't pixel \ncoordinates, it's char position. Every char is \n8x16 pixels in size. Use \\n to change line.\n\n  !Not all chars is supported!\n\nRight now it clear the whole output image as well (Tell me when to remove this).", 10, 2);
		

		gd->Render();

		
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			INPUT.PollEvent(e);
		}

		if (INPUT.GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == Input::InputState::PRESSED)
			lol = false;
	}
		
	delete gd;
	SDL_Quit();
	return 0;
}