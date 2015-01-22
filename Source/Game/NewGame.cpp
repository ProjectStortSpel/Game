#include "GameCreator.h"

#ifdef WIN32
#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#endif

#include "Logger/Logger.h"

int main(int argc, char** argv)
{
	GameCreator* newGame = new GameCreator();

	Logger::GetInstance().AddGroup("ECSL");
	Logger::GetInstance().Log(0, LogSeverity::Info, "Testing1");
	Logger::GetInstance().Log(0, LogSeverity::Info, "Testing2");
	Logger::GetInstance().ChangeFilterFor(0, false);
	Logger::GetInstance().Log(0, LogSeverity::Info, "Testing3");
	Logger::GetInstance().Log(0, LogSeverity::Info, "Testing4");
	Logger::GetInstance().ChangeFilterFor(0, true);
	Logger::GetInstance().Log(0, LogSeverity::Info, "Testing5");
	Logger::GetInstance().Log(0, LogSeverity::Info, "Testing6");
	Logger* lol = &Logger::GetInstance();
	delete(lol);

	newGame->InitializeLua();
	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeNetwork();
	newGame->InitializeWorld("lobby");


	newGame->StartGame(argc, argv);

	delete newGame;
	return 0;
}