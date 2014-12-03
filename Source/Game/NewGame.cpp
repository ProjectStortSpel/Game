#include "GameCreator.h"

#ifdef WIN32
	#include <vld.h>
#endif


#include <glm/common.hpp>


int main(int argc, char** argv)
{
	GameCreator* newGame = new GameCreator();

	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeLua();
	newGame->InitializeWorld();
	


	newGame->StartGame();

	delete newGame;
	return 0;
}