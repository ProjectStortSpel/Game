#include "Game/GameShared.h"


void ClearConsole()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

bool CheckValidMove(std::string _moves, int _cardsToPlay, int _cardsInHand)
{
	if (_moves.size() != _cardsToPlay)
	{
		printf("WRONG SIZE\n");
		return false;
	}
	for (int i = 0; i < _moves.size(); i++)
	{
		int nr = (int)_moves[i] - 48;
		if (nr < 0 || nr >= _cardsInHand)
		{
			printf("WRONG NUMBER\n");
			return false;
		}
		int count = (int)std::count(_moves.begin(), _moves.end(), _moves[i]);
		if (count > 1)
		{
			printf("Does not exist\n");
			return false;
		}
	}
	return true;
}

void PrintString(PacketHandler* _ph, NetConnection* _connection)
{
	printf("%s\n", _ph->ReadString());
}