#include "LuaPotentialFieldHandler.h"

namespace LuaBridge
{
	namespace PotentialField
	{
		std::vector<PF> m_PFs;
		std::vector<PF> m_summedPFs;
		//std::vector<float> m_highestValues;
		std::map<std::string, ObjectType> m_uniqueObjects;
		std::vector<std::vector<float>> m_onTheSpotValues; // The value a field receives on the spot of an item.
		std::vector<std::vector<float>> m_weights;
		std::vector<std::vector<unsigned int>> m_length;
		std::vector<std::vector<unsigned int>> m_power;
		glm::uvec2 m_mapSize;
		int m_maxNoOfAIs;

		void Embed(lua_State* L)
		{
			LuaEmbedder::EmbedClass<PFParam>(L, "PFParam");
			LuaEmbedder::EmbedClassFunction<PFParam>(L, "PFParam", "AddPosition", &PFParam::AddPosition);
			//LuaEmbedder::EmbedClassFunction<PFParam>(L, "PFParam", "SetObject", &PFParam::SetObject);

			LuaEmbedder::AddFunction(L, "InitPFHandler", &LuaInitPFHandler, "PotentialFieldHandler");
			LuaEmbedder::AddFunction(L, "InitPF", &LuaInitPF, "PotentialFieldHandler");
			LuaEmbedder::AddFunction(L, "SumPFs", &LuaSumPFs, "PotentialFieldHandler");
			LuaEmbedder::AddFunction(L, "UpdatePF", &LuaUpdatePF, "PotentialFieldHandler");
			LuaEmbedder::AddFunction(L, "UseMyPF", &LuaUseMyPF, "PotentialFieldHandler");
		}

		int LuaInitPFHandler(lua_State* L)
		{
			m_mapSize.x = LuaEmbedder::PullInt(L, 1);
			m_mapSize.y = LuaEmbedder::PullInt(L, 2);
			m_maxNoOfAIs = LuaEmbedder::PullInt(L, 3);

			//m_uniqueObjects.insert(std::pair<std::string, ObjectType>("NotWalkable", ObjectType::NotWalkable));
			//m_uniqueObjects.insert(std::pair<std::string, ObjectType>("Edge", ObjectType::Unit));
			//m_uniqueObjects.insert(std::pair<std::string, ObjectType>("Hole", ObjectType::Unit));
			m_uniqueObjects.insert(std::pair<std::string, ObjectType>("RiverEnd", ObjectType::Unit));
			m_uniqueObjects.insert(std::pair<std::string, ObjectType>("Unit", ObjectType::Unit));
			m_uniqueObjects.insert(std::pair<std::string, ObjectType>("Void", ObjectType::Void));
			
			/* Initialize the size of the vector containing the PFs and initialize the values to 0.0f.*/
			std::vector<float> tempInner;
			PF tempOuter;
			tempInner.resize(m_mapSize.y, 0.0f);
			tempOuter.resize(m_mapSize.x, tempInner);
			/* Each available ai should have a pf for every object and a sum of these.*/
			m_PFs.resize(m_maxNoOfAIs * (ObjectType::NoOfEnums), tempOuter);
			m_summedPFs.resize(m_maxNoOfAIs, tempOuter);
			//m_highestValues.resize(m_PFs.size());

			/* Initialize the size of the vector containing the "on the spot values".*/
			tempInner.resize(ObjectType::NoOfEnums, 4.0f);
			m_onTheSpotValues.resize(m_maxNoOfAIs, tempInner);
			m_weights.resize(m_maxNoOfAIs, tempInner);

			std::vector<unsigned int> tempIntInner;
			tempIntInner.resize(ObjectType::NoOfEnums, 2);
			m_length.resize(m_maxNoOfAIs, tempIntInner);
			m_power.resize(m_maxNoOfAIs, tempIntInner);

			return 0;
		}

		/* Called from Lua. Inits a potential field.*/
		int LuaInitPF(lua_State* L)
		{
			PFParam* param = LuaEmbedder::PullObject<PFParam>(L, "PFParam", 1);
			int ai = LuaEmbedder::PullInt(L, 2);

			ai -= 1;

			int objectType = m_uniqueObjects.at(LuaEmbedder::PullString(L, 3));
			m_onTheSpotValues[ai][objectType] = LuaEmbedder::PullInt(L, 4);
			m_weights[ai][objectType] = LuaEmbedder::PullInt(L, 5);
			m_length[ai][objectType] = LuaEmbedder::PullInt(L, 6);
			m_power[ai][objectType] = LuaEmbedder::PullInt(L, 7);

			ClearPF(ai, objectType);

			CreatePF(param->m_positions, ai, objectType);

			return 0;
		}

		int LuaUpdatePF(lua_State* L)
		{
			PFParam* param = LuaEmbedder::PullObject<PFParam>(L, "PFParam", 1);
			int ai = LuaEmbedder::PullInt(L, 2);

			ai -= 1;

			int objectType = m_uniqueObjects.at(LuaEmbedder::PullString(L, 3));
			m_onTheSpotValues[ai][objectType] = LuaEmbedder::PullInt(L, 4);
			m_weights[ai][objectType] = LuaEmbedder::PullInt(L, 5);
			m_length[ai][objectType] = LuaEmbedder::PullInt(L, 6);
			m_power[ai][objectType] = LuaEmbedder::PullInt(L, 7);

			ClearPF(ai, objectType);

			CreatePF(param->m_positions, ai, objectType);

			ClearSumPF(ai);

			SumPFs(ai);

			return 0;
		}

		int LuaSumPFs(lua_State* L)
		{
			unsigned int ai = LuaEmbedder::PullInt(L, 1);

			ai -= 1;

			ClearSumPF(ai);

			SumPFs(ai);

			return 0;
		}

		int LuaUseMyPF(lua_State* L)
		{
			int ai = LuaEmbedder::PullInt(L, 1);
			ai -= 1;

			Pathfinder::Instance()->UsePotentialField(&m_summedPFs[ai]);

			return 0;
		}

		void SumPFs(unsigned int _ai)
		{
			for (unsigned int i = 0; i < ObjectType::NoOfEnums; i++)
			{
				for (unsigned int y = 0; y < m_mapSize.y; y++)
				{
					for (unsigned int x = 0; x < m_mapSize.x; x++)
					{
						m_summedPFs[_ai][x][y] += m_PFs[ObjectType::NoOfEnums * _ai + i][x][y];
					}
				}
			}

			//for (unsigned int y = 0; y < m_mapSize.y; y++)
			//{
			//	for (unsigned int x = 0; x < m_mapSize.x; x++)
			//	{
			//		m_summedPFs[_ai][x][y] += m_PFs[ObjectType::NoOfEnums * _ai + i][x][y];
			//	}
			//}

			//for (int y = 0; y < m_mapSize.y; y++)
			//{
			//	for (int x = 0; x < m_mapSize.x; x++)
			//	{
			//		printf("%.1f, ", m_summedPFs[_ai][x][y]);
			//	}
			//	printf("\n\n");
			//}
		}

		void CreatePF(std::vector<glm::uvec2> _positions, unsigned int _ai, unsigned int _objectType)
		{
			//unsigned int offset = (ObjectType::NoOfEnums + 1) * _ai + _objectType;

			for (unsigned int x = 0; x < m_mapSize.x; x++)
			{
				for (unsigned int y = 0; y < m_mapSize.y; y++)
				{
					/* Process every item of the object type. */
					for (unsigned int i = 0; i < _positions.size(); i++)
					{
						int diffx = _positions[i].x - x;
						int diffy = _positions[i].y - y;
						int manhattan = abs(diffx) + abs(diffy);
						//int pfIndex = i + _offset;

						CalcValue(manhattan, x, y, _ai, _objectType);
					}
				}
			}
		}

		void CalcValue(unsigned int _manhattan, unsigned int _x, unsigned int _y, unsigned int _ai, unsigned int _objectType)
		{
			unsigned int offset = ObjectType::NoOfEnums * _ai + _objectType;

			/* Add a value based on the manhattan distance. If we are on the spot, use "m_onTheSpotValue".*/
			if (_manhattan == 0)
			{
				m_PFs[offset][_x][_y] += m_onTheSpotValues[_ai][_objectType];
			}
			else if (_manhattan <= m_length[_ai][_objectType])
			{
				float value = m_weights[_ai][_objectType];
				for (unsigned int i = 0; i < m_power[_ai][_objectType]; i++)
				{
					value /= _manhattan;
				}
				m_PFs[offset][_x][_y] += value;
			}

			/* Save the highest values for each field to normalize later.*/
			//if (m_PFs[_index][_x][_y] > m_highestValues[_index])
			//{
			//	m_highestValues[_index] = m_PFs[_index][_x][_y];
			//}
		}

		void ClearPF(unsigned int _ai, unsigned int _objectType)
		{
			unsigned int offset = ObjectType::NoOfEnums * _ai + _objectType;

			for (unsigned int x = 0; x < m_mapSize.x; x++)
			{
				for (unsigned int y = 0; y < m_mapSize.y; y++)
				{
					m_PFs[offset][x][y] = 0.0f;
				}
			}
		}

		void ClearSumPF(unsigned int _ai)
		{
			for (unsigned int x = 0; x < m_mapSize.x; x++)
			{
				for (unsigned int y = 0; y < m_mapSize.y; y++)
				{
					m_summedPFs[_ai][x][y] = 0.0f;
				}
			}
		}

		/* Normalize the field so that the highest value is 1.*/
		//void NormalizePF(unsigned int _offset)
		//{
		//	for (unsigned int i = 0; i < m_PFs.size(); i++)
		//	{
		//		for (unsigned int x = 0; x < m_mapSize.x; x++)
		//		{
		//			for (unsigned int y = 0; y < m_mapSize.y; y++)
		//			{
		//				m_PFs[i][x][y] /= m_highestValues[i];
		//			}
		//		}
		//	}
		//}
	}
}