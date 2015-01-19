#include "GameCreator.h"

#ifdef WIN32
#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#endif

int main(int argc, char** argv)
{
	GameCreator* newGame = new GameCreator();

	newGame->InitializeLua();
	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeNetwork();
	newGame->InitializeWorld("lobby");


	newGame->StartGame(argc, argv);

	delete newGame;
	return 0;
}