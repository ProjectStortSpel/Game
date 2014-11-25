#ifndef GAMESHARED_H
#define GAMESHARED_H

#include <string>

#include "Network/PacketHandler.h"

void ClearConsole();

bool CheckValidMove(std::string _moves, int _cardsToPlay, int _cardsInHand);

void PrintString(PacketHandler* _ph, NetConnection* _connection);

struct MovementCard
{
	std::string name;
	float prio;
	MovementCard()
	{

	}
	MovementCard(std::string n, float p)
	{
		name = n;
		prio = p;
	}
};

#endif