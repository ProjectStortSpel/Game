#include "GameCreator.h"

#ifdef WIN32
	#include <vld/vld.h>
#endif


#include <glm/common.hpp>


int main(int argc, char** argv)
{
	GameCreator* newGame = new GameCreator();

	newGame->InitializeLua();
	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeWorld();
	


	newGame->StartGame();

	delete newGame;
	return 0;
}