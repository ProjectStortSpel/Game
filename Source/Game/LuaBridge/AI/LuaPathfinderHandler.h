#ifndef PATHFINDERHANDLER_H
#define PATHFINDERHANDLER_H

#include <LuaEmbedder/LuaEmbedder.h>
#include <vector>
#include "Game/Pathfinder.h"
#include "Game/LuaBridge/AI/LuaPotentialFieldHandler.h"

namespace LuaBridge
{
	namespace PathfinderHandler
	{
		struct InputData
		{
			std::vector<float> walking_cost;
			std::vector<bool> walkable;
			int x, y;

			InputData(lua_State* _l)
			{
			}

			int AddElement(lua_State* _l)
			{
				float w_c	= LuaEmbedder::PullFloat(_l, 1);
				bool w		= LuaEmbedder::PullBool(_l, 2);

				walking_cost.push_back(w_c);
				walkable.push_back(w);
				return 0;
			}

			int SetSize(lua_State* _l)
			{
				x = LuaEmbedder::PullInt(_l, 1);
				y = LuaEmbedder::PullInt(_l, 2);
				return 0;
			}

		};

		void	Embed(lua_State* _l);
		
		/*
			SetPathfinderTurningCost:
			float		: turning cost
		*/
		int		SetPathfinderTurningCost(lua_State* _l);

		/*
			SetPathfinderData:
			bool[x][y]	: walkable
			float		: walking cost
			int (x, y)	: position
		*/
		int		SetPathfinderData(lua_State* _l);

		/*
			GeneratePathfinderPath:
			int (x, y)	: start position
			int (x, y)	: goal position
		*/
		int		GeneratePathfinderPath(lua_State* _l);

		
	}
}
#endif // PATHFINDERHANDLER_H