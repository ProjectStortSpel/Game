#include "DataSlimArray.h"

#include <cstdlib>
#include <cstring>

namespace ECSL
{
	DataSlimArray::DataSlimArray(unsigned const int _rowCount, unsigned const int _bytesPerRow)
	{
		m_bytesPerRow = _bytesPerRow;
		m_rowCount = _rowCount;
		m_dataTable = new char*[_rowCount];
		for (unsigned int i = 0; i < m_rowCount; ++i)
			m_dataTable[i] = nullptr;
	}

	DataSlimArray::~DataSlimArray()
	{
		Release();
	}

	void DataSlimArray::Release()
	{
		if (m_dataTable != 0)
		{
			for (unsigned int i = 0; i < m_rowCount; ++i)
				if (m_dataTable[i])
					delete(m_dataTable[i]);
			delete(m_dataTable);
		}
	}

	void DataSlimArray::ClearRow(unsigned const int _row)
	{
		delete(m_dataTable[_row]);
		m_dataTable[_row] = nullptr;
	}

	void DataSlimArray::ClearTable()
	{
		for (unsigned int i = 0; i < m_rowCount; ++i)
		{
			if (m_dataTable[i])
			{
				delete(m_dataTable[i]);
				m_dataTable[i] = nullptr;
			}
		}
	}

	DataLocation DataSlimArray::GetData(unsigned const int _row)
	{
		if (!m_dataTable[_row])
			m_dataTable[_row] = new char[m_bytesPerRow];
		return m_dataTable[_row];
	}

	DataLocation DataSlimArray::GetData(unsigned const int _row, unsigned const int _column)
	{
		if (!m_dataTable[_row])
			m_dataTable[_row] = new char[m_bytesPerRow];
		return (m_dataTable[_row] + _column);
	}

	void DataSlimArray::SetData(unsigned const int _row, void* _data, unsigned const int _byteCount)
	{
		if (!m_dataTable[_row])
			m_dataTable[_row] = new char[m_bytesPerRow];
		memcpy(m_dataTable[_row], _data, _byteCount);
	}

	void DataSlimArray::SetData(unsigned const int _row, unsigned const int _column, void* _data, unsigned const int _byteCount)
	{
		if (!m_dataTable[_row])
			m_dataTable[_row] = new char[m_bytesPerRow];
		memcpy(m_dataTable[_row] + _column, _data, _byteCount);
	}

	const unsigned int DataSlimArray::GetRowCount() const
	{
		return m_rowCount;
	}

	const unsigned int DataSlimArray::GetBytesPerRow() const
	{
		return m_bytesPerRow;
	}

	const unsigned int DataSlimArray::GetMemoryAllocated() const
	{
		unsigned int memory = 0;
		for (unsigned int i = 0; i < m_rowCount; ++i)
			if (m_dataTable[i] != 0)
				memory += m_bytesPerRow;
		return sizeof(DataSlimArray) + (m_rowCount * sizeof(char*)) + memory;
	}
}