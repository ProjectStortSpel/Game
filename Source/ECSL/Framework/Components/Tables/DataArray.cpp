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

	void DataArray::ClearRow(unsigned const int _row)
	{
		memset(m_dataTable + (_row * m_bytesPerRow), 0, m_bytesPerRow);
	}

	void DataArray::ClearTable()
	{
		memset(m_dataTable, 0, (m_rowCount * m_bytesPerRow));
	}

	const unsigned int DataArray::GetRowCount() const
	{
		return m_rowCount;
	}

	const unsigned int DataArray::GetBytesPerRow() const
	{
		return m_bytesPerRow;
	}

	const unsigned int DataArray::GetMemoryAllocated() const
	{
		return sizeof(DataArray) + m_bytesPerRow * m_rowCount;
	}
}
