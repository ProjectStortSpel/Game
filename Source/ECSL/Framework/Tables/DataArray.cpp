#include "DataArray.h"

#include <cstdlib>
#include <cstring>

namespace ECSL
{
	DataArray::DataArray(unsigned const int _rowCount, unsigned const int _bytesPerRow)
	{
		m_bytesPerRow = _bytesPerRow;
		m_rowCount = _rowCount;
		m_dataTable = (char*)calloc(m_rowCount * m_bytesPerRow, sizeof(char));
	}

	DataArray::~DataArray()
	{
		Release();
	}

	void DataArray::Release()
	{
		if (m_dataTable != 0)
			free(m_dataTable);
	}

	void DataArray::ClearData()
	{
		Release();
		m_dataTable = new char[m_rowCount * m_bytesPerRow];
	}

	DataLocation DataArray::GetData(unsigned const int _id) const
	{ 
		return m_dataTable + (_id * m_bytesPerRow);
	}

	DataLocation DataArray::GetData(unsigned const int _id, unsigned const int _index) const
	{ 
		return (m_dataTable + (_id * m_bytesPerRow) + _index); 
	}

	void DataArray::SetData(unsigned const int _id, void* _data, unsigned const int _byteCount)
	{
		memcpy(m_dataTable + (_id * m_bytesPerRow), _data, _byteCount);
	}

	void DataArray::SetData(unsigned const int _id, unsigned const int _index, void* _data, unsigned const int _byteCount)
	{
		memcpy(m_dataTable + (_id * m_bytesPerRow) + _index, _data, _byteCount);
	}

	const int DataArray::GetRowCount() const
	{
		return m_rowCount;
	}

	const int DataArray::GetBytesPerRow() const
	{
		return m_bytesPerRow;
	}

	const int DataArray::GetMemoryAllocated() const
	{
		return sizeof(DataArray) + m_bytesPerRow * m_rowCount;
	}
}
