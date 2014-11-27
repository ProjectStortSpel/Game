#include "DataArray.h"

#include <assert.h>
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
		m_dataTable = (char*)calloc(m_rowCount * m_bytesPerRow, sizeof(char));
	}

	DataLocation DataArray::GetData(unsigned const int _row) const
	{ 
		return m_dataTable + (_row * m_bytesPerRow);
	}

	DataLocation DataArray::GetData(unsigned const int _row, unsigned const int _column) const
	{ 
		return (m_dataTable + (_row * m_bytesPerRow) + _column);
	}

	void DataArray::SetData(unsigned const int _row, void* _data, unsigned const int _byteCount)
	{
		memcpy(m_dataTable + (_row * m_bytesPerRow), _data, _byteCount);
	}

	void DataArray::SetData(unsigned const int _row, unsigned const int _column, void* _data, unsigned const int _byteCount)
	{
		memcpy(m_dataTable + (_row * m_bytesPerRow) + _column, _data, _byteCount);
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
