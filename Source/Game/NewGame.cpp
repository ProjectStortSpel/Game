#include "GameCreator.h"

#ifdef WIN32
	#include <VLD/vld.h>
#endif


#include <glm/common.hpp>
#include "ECSL/Framework/Parsing/EntityTemplateReader.h"
#include "ECSL/Framework/Common/FileHelper.h"


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