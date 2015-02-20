#ifndef DATASLIMARRAY_H
#define DATASLIMARRAY_H

#include <SDL/SDL.h>
#include <vector>
#include "DataTable.h"

namespace ECSL
{
	class DECLSPEC DataSlimArray : public DataTable
	{
	public:
		DataSlimArray(unsigned const int _rowCount, unsigned const int _bytesPerRow);
		~DataSlimArray();

		void Release();
		void ClearRow(unsigned const int _row);
		void ClearTable();

		DataLocation GetData(unsigned const int _id);
		DataLocation GetData(unsigned const int _id, unsigned const int _index);

		void SetData(unsigned const int _id, void* _data, unsigned const int _byteCount);
		void SetData(unsigned const int _id, unsigned const int _index, void* _data, unsigned const int _byteCount);

		const unsigned int GetRowCount() const;

		/* Data usage */
		const unsigned int GetBytesPerRow() const;
		const unsigned int GetMemoryAllocated() const;

	private:
		char** m_dataTable;
		unsigned int m_bytesPerRow;
		unsigned int m_rowCount;
	};
}
#endif