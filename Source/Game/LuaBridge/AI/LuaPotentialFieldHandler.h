#ifndef POTENTIALFIELDHANDLER_H
#define POTENTIALFIELDHANDLER_H

#include "Input/InputWrapper.h"
#include <LuaEmbedder/LuaEmbedder.h>
#include <glm/glm.hpp>
#include <vector>

namespace LuaBridge
{
	namespace PotentialField
	{
		typedef std::vector<std::vector<float>> PF;

		/* If something else should be added, don't forget to add it into the map in InitPFHandler.*/
		enum ObjectType
		{
			NotWalkable,
			Unit,
			Void,
			Last = Void
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
			std::string m_object;
			std::vector<glm::uvec2> m_positions;

			PFParam(lua_State* L) { }

			int AddPosition(lua_State* L)
			{
				glm::uvec2 pos = glm::uvec2(LuaEmbedder::PullInt(L, 1), LuaEmbedder::PullInt(L, 2));
				m_positions.push_back(pos);
				return 0;
			}

			int SetObject(lua_State* L)
			{
				m_object = LuaEmbedder::PullString(L, 1);
				return 0;
			}
		};

		extern std::vector<PF> m_PFs;
		extern std::vector<float> m_highestValues;
		extern std::map<std::string, ObjectType> m_uniqueObjects;
		extern std::vector<float> m_onTheSpotValues; // The value a field receives on the spot of an item.
		extern glm::uvec2 m_mapSize;

		void Embed(lua_State* L);
		//int InitPFHandler(lua_State* L);
		int InitPF(lua_State* L);
		int UpdatePF(lua_State* L);

		//const PF* GetPF(ObjectType _object);
		//const PF* GetPF(int _object);
		const PF* GetPF(std::string _object);

		//void NormalizePF(unsigned int _offset);
		void InitPFHandler(unsigned int _mapSizeX, unsigned int _mapSizeY);
		void CalcValue(unsigned int _manhattan, unsigned int _x, unsigned int _y, unsigned int _offset);
		void CreatePF(std::vector<glm::uvec2> m_positions, unsigned int _offset);
		void ClearPF(unsigned int _offset);
	}
}

#endif