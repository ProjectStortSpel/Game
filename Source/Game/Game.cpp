#include <SDL/SDL.h>
#include "ECSL/ECSL.h"
#include "Network/Server.h"
#include "Network/Client.h"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	Server s;
	s.Start();

	Client c;
	c.Connect();

	system("pause");

	SDL_Quit();
	return 0;
}