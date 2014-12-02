#ifndef DATAARRAY_H
#define DATAARRAY_H

#include <SDL/SDL.h>
#include "DataTable.h"

namespace ECSL
{
	class DECLSPEC DataArray : public DataTable
	{
	public:
		DataArray(unsigned const int _rowCount, unsigned const int _bytesPerRow);
		~DataArray();
		
		void Release();
		void ClearRow(unsigned const int _row);
		void ClearTable();


		inline DataLocation GetData(unsigned const int _row) const
		{
			return m_dataTable + (_row * m_bytesPerRow);
		}
		inline DataLocation GetData(unsigned const int _row, unsigned const int _column) const
		{
			return (m_dataTable + (_row * m_bytesPerRow) + _column);
		}

		inline virtual void SetData(unsigned const int _row, void* _data, unsigned const int _byteCount)
		{
			memcpy(m_dataTable + (_row * m_bytesPerRow), _data, _byteCount);
		}

		inline virtual void SetData(unsigned const int _row, unsigned const int _column, void* _data, unsigned const int _byteCount)
		{
			memcpy(m_dataTable + (_row * m_bytesPerRow) + _column, _data, _byteCount);
		}

		const unsigned int GetRowCount() const;

		/* Data usage */
		const unsigned int GetBytesPerRow() const;
		const unsigned int GetMemoryAllocated() const;

	private:
		char* m_dataTable;
		unsigned int m_bytesPerRow;
		unsigned int m_rowCount;
	};
}
#endif