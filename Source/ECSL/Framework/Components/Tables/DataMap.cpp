#include "DataMap.h"

#include <assert.h>
#include <cstdlib>
#include <cstring>

namespace ECSL
{
	DataMap::DataMap(unsigned const int _bytesPerRow)
	{
		m_bytesPerRow = _bytesPerRow;
		m_dataMap = new std::map<unsigned int, char*>();
	}

	DataMap::~DataMap()
	{
		Release();
	}

	void DataMap::Release()
	{
		if (m_dataMap != 0)
		{
			for (auto it = m_dataMap->begin(); it != m_dataMap->end(); ++it)
				delete it->second;
			delete(m_dataMap);
		}
			
	}

	void DataMap::ClearRow(unsigned const int _row)
	{
		m_dataMap->erase(m_dataMap->find(_row));
	}

	void DataMap::ClearTable()
	{
		Release();
		m_dataMap = new std::map<unsigned int, char*>();
	}

	DataLocation DataMap::GetData(unsigned const int _id)
	{
		return (*m_dataMap)[_id];
	}

	DataLocation DataMap::GetData(unsigned const int _id, unsigned const int _index)
	{
		return (*m_dataMap)[_id] + _index;
	}

	void DataMap::SetData(unsigned const int _id, void* _data, unsigned const int _byteCount)
	{
		if ((*m_dataMap).find(_id) == (*m_dataMap).end())
			(*m_dataMap)[_id] = new char[m_bytesPerRow];

		memcpy((*m_dataMap)[_id], _data, _byteCount);
	}

	void DataMap::SetData(unsigned const int _id, unsigned const int _index, void* _data, unsigned const int _byteCount)
	{
		if ((*m_dataMap).find(_id) == (*m_dataMap).end())
			(*m_dataMap)[_id] = new char[m_bytesPerRow];

		memcpy((*m_dataMap)[_id] + _index, _data, _byteCount);
	}

	const unsigned int DataMap::GetRowCount() const
	{
		return (unsigned int)m_dataMap->size();
	}

	const unsigned int DataMap::GetBytesPerRow() const
	{
		return m_bytesPerRow;
	}

	const unsigned int DataMap::GetMemoryAllocated() const
	{
		return (unsigned int)(sizeof(DataMap) + m_bytesPerRow * m_dataMap->max_size());
	}
}
