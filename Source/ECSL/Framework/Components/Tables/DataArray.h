#ifndef DATAARRAY_H
#define DATAARRAY_H

#include <SDL/SDL.h>
#include "DataTable.h"

namespace ECSL
{
	class DECLSPEC DataArray : public DataTable
	{
	private:
		char* m_dataTable;
		unsigned int m_bytesPerRow;
		unsigned int m_rowCount;

	public:
		DataArray(unsigned const int _rowCount, unsigned const int _bytesPerRow);
		~DataArray();
		
		void Release();
		void ClearRow(unsigned const int _row);
		void ClearTable();

		inline DataLocation GetData(unsigned const int _row) const;
		inline DataLocation GetData(unsigned const int _row, unsigned const int _column) const;
		inline virtual void SetData(unsigned const int _row, void* _data, unsigned const int _byteCount);
		inline virtual void SetData(unsigned const int _row, unsigned const int _column, void* _data, unsigned const int _byteCount);

		const int GetRowCount() const;

		/* Data usage */
		const int GetBytesPerRow() const;
		const int GetMemoryAllocated() const;
	};
}
#endif