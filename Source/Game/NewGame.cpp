#include "GameCreator.h"

#ifdef WIN32
#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#endif

#include "Logger/Logger.h"
#include "Game/Network/ClientDatabase.h"

int main(int argc, char** argv)
{
	Logger::GetInstance().AddGroup("Game");
	GameCreator* newGame = new GameCreator();


	newGame->InitializeLua();
	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeNetwork();
	newGame->InitializeWorld("lobbyNew");


	newGame->StartGame(argc, argv);
	delete newGame;

	delete(&ClientDatabase::GetInstance());
	delete(&Logger::GetInstance());
	
	return 0;
}