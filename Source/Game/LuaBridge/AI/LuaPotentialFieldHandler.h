#ifndef POTENTIALFIELDHANDLER_H
#define POTENTIALFIELDHANDLER_H


#include <LuaEmbedder/LuaEmbedder.h>
#include <glm/glm.hpp>
#include <vector>
#include "Game/Pathfinder.h"

namespace LuaBridge
{
	namespace PotentialField
	{
		typedef std::vector<std::vector<float>> PF;

		/* If something else should be added, don't forget to add it into the map in InitPFHandler.*/
		enum ObjectType
		{
			NotWalkable,
			//Edge,
			//Hole,
			//RiverEnd,
			Unit,
			Void,
			NoOfEnums
		};

		//struct uint2
		//{
		//	unsigned int x, y;

		//	uint2()
		//	{
		//		x = 0;
		//		y = 0;
		//	} 

		//	uint2(unsigned int _x, unsigned int _y)
		//	{
		//		x = _x;
		//		y = _y;
		//	}
		//};

		struct PFParam
		{
			//std::string m_object;
			std::vector<glm::uvec2> m_positions;

			PFParam(lua_State* L) { }

			int AddPosition(lua_State* L)
			{
				unsigned int x = LuaEmbedder::PullInt(L, 1);
				unsigned int y = LuaEmbedder::PullInt(L, 2);
				glm::uvec2 pos = glm::uvec2(x, y);
				m_positions.push_back(pos);
				return 0;
			}

			//int SetObject(lua_State* L)
			//{
			//	m_object = LuaEmbedder::PullString(L, 1);
			//	return 0;
			//}
		};

		extern std::vector<PF> m_PFs;
		extern std::vector<PF> m_summedPFs;
		//extern std::vector<float> m_highestValues;
		extern std::map<std::string, ObjectType> m_uniqueObjects;
		extern std::vector<std::vector<float>> m_onTheSpotValues; // The value a field receives on the spot of an item.
		extern std::vector<std::vector<float>> m_weights;
		extern std::vector<std::vector<unsigned int>> m_length; // How far the pf reaches.
		extern std::vector<std::vector<unsigned int>> m_power; // How much the pf decreases by distance.
		extern glm::uvec2 m_mapSize;
		extern int m_maxNoOfAIs;

		void Embed(lua_State* L);
		int LuaInitPFHandler(lua_State* L);
		int LuaInitPF(lua_State* L);
		int LuaSumPFs(lua_State* L);
		int LuaUpdatePF(lua_State* L);
		int LuaUseMyPF(lua_State* L);

		//const PF* GetPF(ObjectType _object);
		//const PF* GetPF(int _object);
		//const PF* GetPF(std::string _object);

		//void NormalizePF(unsigned int _offset);
		//void InitPFHandler(unsigned int _mapSizeX, unsigned int _mapSizeY);
		void CalcValue(unsigned int _manhattan, unsigned int _x, unsigned int _y, unsigned int _ai, unsigned int _objectType);
		void SumPFs(unsigned int _ai);
		void CreatePF(std::vector<glm::uvec2> _positions, unsigned int _ai, unsigned int _objectType);
		void ClearPF(unsigned int _ai, unsigned int _objectType);
		void ClearSumPF(unsigned int _ai);
	}
}

#endif