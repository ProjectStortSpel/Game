#include "GameCreator.h"

#ifdef WIN32
	#include <VLD/vld.h>
#endif

int main(int argc, char** argv)
{
	GameCreator* newGame = new GameCreator();

	newGame->InitializeLua();
	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeNetwork();
	newGame->InitializeWorld("storaspel");


	newGame->StartGame();

	delete newGame;
	return 0;
}