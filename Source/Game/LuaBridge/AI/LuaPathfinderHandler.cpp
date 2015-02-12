#include "Game/LuaBridge/AI/LuaPathfinderHandler.h"

namespace LuaBridge
{
	namespace PathfinderHandler
	{
		void Embed(lua_State* _l)
		{
			LuaEmbedder::EmbedClass<InputData>(_l, "InputData");
			LuaEmbedder::EmbedClassFunction<InputData>(_l, "InputData", "AddTile", &InputData::AddElement);
			LuaEmbedder::EmbedClassFunction<InputData>(_l, "InputData", "SetSize", &InputData::SetSize);

			LuaEmbedder::AddFunction(_l, "SetTurningCost", &SetPathfinderTurningCost, "PathfinderHandler");
			LuaEmbedder::AddFunction(_l, "SetTileWalkable", &SetTileWalkable, "PathfinderHandler");
			LuaEmbedder::AddFunction(_l, "SetData", &SetPathfinderData, "PathfinderHandler");
			LuaEmbedder::AddFunction(_l, "GeneratePath", &GeneratePathfinderPath, "PathfinderHandler");
		}

		int		SetPathfinderTurningCost(lua_State* _l)
		{
			int ret_value = 0;
			float turning_cost = LuaEmbedder::PullFloat(_l, 1);

			Pathfinder* pathfinder = Pathfinder::Instance();

			return ret_value;
		}

		int		SetPathfinderData(lua_State* _l)
		{
			int ret_value = 0;
			Pathfinder* pathfinder = Pathfinder::Instance();

			InputData *value = LuaEmbedder::PullObject<InputData>(_l, "InputData", 1);
			int x_size = value->x;
			int y_size = value->y;

			/*
				kalla init för PF 	
			*/

			std::vector<float>	walk_cost	= value->walking_cost;
			std::vector<bool>	walkable	= value->walkable;

			//allocate memory </3 </3
			tile_data** data;
			
			data = new tile_data*[x_size];
			for (int i = 0; i < x_size; ++i)
			{
				data[i] = new tile_data[y_size];
			}

			for (int y = 0; y < y_size; ++y)
			{
				for (int x = 0; x < x_size; ++x)
				{
					data[x][y].position = coord(x, y);
					data[x][y].walkable = walkable[y*x_size + x];
					data[x][y].walking_cost = walk_cost[y*x_size + x];
				}
			}

			pathfinder->SetNodeData((const tile_data***)(&data), x_size, y_size);

			//clean memory <3<3
			for (int i = 0; i < x_size; ++i)
			{
				delete(data[i]);
			}
			delete(data);
			return ret_value;
		}

		int		GeneratePathfinderPath(lua_State* _l)
		{
			int ret_value = 0;

			Pathfinder* pathfinder = Pathfinder::Instance();

			coord start = coord(LuaEmbedder::PullInt(_l, 1), LuaEmbedder::PullInt(_l, 2));
			coord goal = coord(LuaEmbedder::PullInt(_l, 3), LuaEmbedder::PullInt(_l, 4));
			ret_value = pathfinder->GeneratePath(start, goal).size();
			if (ret_value == 0)
			{
				ret_value = INT_MAX;
			}
			LuaEmbedder::PushInt(_l, ret_value);
			return 1;
		}

		int		SetTileWalkable(lua_State* _l)
		{
			int ret_value = 1;
			int x			= LuaEmbedder::PullInt(_l, 1);
			int y			= LuaEmbedder::PullInt(_l, 2);
			bool walkable	= LuaEmbedder::PullBool(_l, 3);
			
			//printf("sending data to pathfinder: (%i,%i), %s", x, y, walkable ? "True" : "False");

			Pathfinder* pathfinder = Pathfinder::Instance();

			pathfinder->ChangeWalkable(x, y, walkable);

			LuaEmbedder::PushInt(_l, ret_value);
			return ret_value;
		}
	}
}