#include "GameCreator.h"

#ifdef WIN32
	#define _CRTDBG_MAP_ALLOC

	#ifdef _DEBUG
		#ifndef DBG_NEW
			#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			#define new DBG_NEW
		#endif
	#endif

	#include <stdlib.h>
	#include <crtdbg.h>
#endif





int main(int argc, char** argv)
{
	GameCreator* newGame = new GameCreator();

	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeWorld();


	newGame->StartGame();

	delete newGame;
#ifdef WIN32
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}