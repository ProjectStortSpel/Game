#ifndef DATAMAP_H
#define DATAMAP_H

#include <SDL/SDL.h>
#include <map>
#include "DataTable.h"

namespace ECSL
{
	class DECLSPEC DataMap : public DataTable
	{
	public:
		DataMap(unsigned const int _bytesPerRow);
		~DataMap();

		void Release();
		void ClearRow(unsigned const int _row);
		void ClearTable();

		const unsigned int GetRowCount() const;

		inline DataLocation GetData(unsigned const int _id) const
		{
			return (*m_dataMap)[_id];
		}
		inline DataLocation GetData(unsigned const int _id, unsigned const int _index) const
		{
			return (*m_dataMap)[_id] + _index;
		}
		inline void SetData(unsigned const int _id, void* _data, unsigned const int _byteCount)
		{
			if ((*m_dataMap).find(_id) == (*m_dataMap).end())
				(*m_dataMap)[_id] = new char[m_bytesPerRow];

			memcpy((*m_dataMap)[_id], _data, _byteCount);
		}
		inline void SetData(unsigned const int _id, unsigned const int _index, void* _data, unsigned const int _byteCount)
		{
			if ((*m_dataMap).find(_id) == (*m_dataMap).end())
				(*m_dataMap)[_id] = new char[m_bytesPerRow];

			memcpy((*m_dataMap)[_id] + _index, _data, _byteCount);
		}

		/* Data usage */
		const unsigned int GetBytesPerRow() const;
		const unsigned int GetMemoryAllocated() const;

	private:
		std::map<unsigned int, char*>* m_dataMap;
		unsigned int m_bytesPerRow;
	};
}
#endif