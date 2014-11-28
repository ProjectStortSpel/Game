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
	void DataMap::ClearTable()
	{
		Release();
		m_dataMap = new std::map<unsigned int, char*>();
	}
	void DataMap::ClearRow(unsigned const int _row)
	{
		m_dataMap->erase(m_dataMap->find(_row));
	}

	DataLocation DataMap::GetData(unsigned const int _id) const
	{
		return (*m_dataMap)[_id];
	}
	DataLocation DataMap::GetData(unsigned const int _id, unsigned const int _index) const
	{
		return (*m_dataMap)[_id] + _index;
	}
	void DataMap::SetData(unsigned const int _id, void* _data, unsigned const int _byteCount)
	{
		memcpy((*m_dataMap)[_id], _data, _byteCount);
	}
	void DataMap::SetData(unsigned const int _id, unsigned const int _index, void* _data, unsigned const int _byteCount)
	{
		memcpy((*m_dataMap)[_id] + _index, _data, _byteCount);
	}

	const int DataMap::GetRowCount() const
	{
		return m_dataMap->size();
	}

	const int DataMap::GetBytesPerRow() const
	{
		return m_bytesPerRow;
	}

	const int DataMap::GetMemoryAllocated() const
	{
		return sizeof(DataMap) + m_bytesPerRow * m_dataMap->size();
	}
}
