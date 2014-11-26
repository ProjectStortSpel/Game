#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include <vector>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	//ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	ECSL::Parser* parser = new ECSL::Parser();
	ECSL::Section* haha = parser->ParseFile("content/components/component.cmp");
	delete(parser);
	delete(haha);
	//worldInitializer.Add<int>();
	//ECSL::World* world = worldCreator.InitializeWorld(100);
	SDL_Quit();
	system("pause");
	_CrtDumpMemoryLeaks();
	return 0;
}