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
		void ClearData();

		inline DataLocation GetData(unsigned const int _id) const;
		inline DataLocation GetData(unsigned const int _id, unsigned const int _index) const;
		inline virtual void SetData(unsigned const int _id, void* _data, unsigned const int _byteCount);
		inline virtual void SetData(unsigned const int _id, unsigned const int _index, void* _data, unsigned const int _byteCount);

		const int GetRowCount() const;

		/* Data usage */
		const int GetBytesPerRow() const;
		const int GetMemoryAllocated() const;
	};
}
#endif