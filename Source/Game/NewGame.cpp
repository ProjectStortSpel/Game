#include "GameCreator.h"

#ifdef WIN32
#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#endif

int main(int argc, char** argv)
{
	GameCreator* newGame = new GameCreator();
	char* lol = new char[10];
	for (int i = 0; i < 10; ++i)
		lol[i] = 'a';
	lol[9] = '\0';
	char* lal = new char[10];
	memcpy_s(lal, 10, lol, 10);

	newGame->InitializeLua();
	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeNetwork();
	newGame->InitializeWorld("lobby");


	newGame->StartGame(argc, argv);

	delete newGame;
	return 0;
}