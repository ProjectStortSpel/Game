#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include <vector>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	ECSL::Parser* parser = new ECSL::Parser();
	parser->ParseFile("content/components/component.cmp");
	delete(parser);
	//worldInitializer.Add<int>();
	ECSL::World* world = worldCreator.InitializeWorld(100);
	SDL_Quit();
	system("pause");
	return 0;
}