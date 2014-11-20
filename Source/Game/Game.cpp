#include <SDL/SDL.h>
#include <ECSL/ECSL.h>
#include <vector>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	ECSL::WorldInitializer worldInitializer = ECSL::WorldInitializer();
	ECSL::Parser parser = ECSL::Parser();
	//worldInitializer.Add<int>();
	ECSL::World* world = worldInitializer.InitializeWorld(100);
	SDL_Quit();
	return 0;
}