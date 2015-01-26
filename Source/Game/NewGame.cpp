#include "GameCreator.h"

#ifdef WIN32
#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#endif

#include "Logger/Logger.h"

int main(int argc, char** argv)
{
	Logger::GetInstance().AddGroup("Game");
	GameCreator* newGame = new GameCreator();


	newGame->InitializeLua();
	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeNetwork();
	newGame->InitializeWorld("lobby");


	newGame->StartGame(argc, argv);
	delete newGame;

	delete(&Logger::GetInstance());
	return 0;
}