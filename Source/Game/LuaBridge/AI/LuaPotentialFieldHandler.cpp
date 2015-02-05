#include "LuaPotentialFieldHandler.h"

namespace LuaBridge
{
	namespace PotentialField
	{
		std::vector<PF> m_PFs;
		std::vector<float> m_highestValues;
		std::map<std::string, ObjectType> m_uniqueObjects;
		std::vector<float> m_onTheSpotValues; // The value a field receives on the spot of an item.
		glm::uvec2 m_mapSize;

		void Embed(lua_State* L)
		{
			LuaEmbedder::EmbedClass<PFParam>(L, "PFParam");
			LuaEmbedder::EmbedClassFunction<PFParam>(L, "PFParam", "AddPosition", &PFParam::AddPosition);
			LuaEmbedder::EmbedClassFunction<PFParam>(L, "PFParam", "SetObject", &PFParam::SetObject);

			//LuaEmbedder::AddFunction(L, "InitPFHandler", &InitPFHandler, "PotentialField");
			LuaEmbedder::AddFunction(L, "InitPF", &InitPF, "PotentialField");
			LuaEmbedder::AddFunction(L, "UpdatePF", &UpdatePF, "PotentialField");
		}

		void InitPFHandler(unsigned int _mapSizeX, unsigned int _mapSizeY)
		{
			m_mapSize.x = _mapSizeX;
			m_mapSize.y = _mapSizeY;

			m_uniqueObjects.insert(std::pair<std::string, ObjectType>("NotWalkable", ObjectType::NotWalkable));
			m_uniqueObjects.insert(std::pair<std::string, ObjectType>("Unit", ObjectType::Unit));
			m_uniqueObjects.insert(std::pair<std::string, ObjectType>("Void", ObjectType::Void));

			for (int i = 0; i < m_uniqueObjects.size(); i++)
			{
				m_onTheSpotValues[i] = 4.0f;
			}

			/* Initialize the size of the vector containing the PFs and initialize the values to 0.0f.*/
			std::vector<float> tempInner;
			PF tempOuter;
			tempInner.resize(m_mapSize.y, 0.0f);
			tempOuter.resize(m_mapSize.x, tempInner);
			m_PFs.resize(ObjectType::Last + 1, tempOuter);
			m_highestValues.resize(m_PFs.size());
		}

		int InitPF(lua_State* L)
		{
			PFParam* param = LuaEmbedder::PullObject<PFParam>(L, "PFParam", 1);
			
			int objectType = m_uniqueObjects.at(param->m_object);

			CreatePF(param->m_positions, objectType);

			return 0;
		}

		int UpdatePF(lua_State* L)
		{
			PFParam* param = LuaEmbedder::PullObject<PFParam>(L, "PFParam", 1);

			int objectType = m_uniqueObjects.at(param->m_object);

			ClearPF(objectType);

			CreatePF(param->m_positions, objectType);

			return 0;
		}

		void CreatePF(std::vector<glm::uvec2> _positions, unsigned int _offset)
		{
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

						CalcValue(manhattan, x, y, _offset);
					}
				}
			}
		}

		void CalcValue(unsigned int _manhattan, unsigned int _x, unsigned int _y, unsigned int _index)
		{
			/* Add a value based on the manhattan distance. If we are on the spot, use "m_onTheSpotValue".*/
			if (_manhattan == 0)
			{
				m_PFs[_index][_x][_y] += m_onTheSpotValues[_index];
			}
			else
			{
				m_PFs[_index][_x][_y] += 1.0f / (_manhattan * _manhattan);
			}

			/* Save the highest values for each field to normalize later.*/
			if (m_PFs[_index][_x][_y] > m_highestValues[_index])
			{
				m_highestValues[_index] = m_PFs[_index][_x][_y];
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

		void ClearPF(unsigned int _offset)
		{
			for (unsigned int x = 0; x < m_mapSize.x; x++)
			{
				for (unsigned int y = 0; y < m_mapSize.y; y++)
				{
					m_PFs[_offset][x][y] = 0.0f;
				}
			}
		}

		const PF* GetPF(std::string _object) 
		{ 
			return &(m_PFs[m_uniqueObjects.at(_object)]); 
		}
	}
}