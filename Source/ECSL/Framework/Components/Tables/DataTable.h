#ifndef DATATABLE_H
#define DATATABLE_H

#include <SDL/SDL.h>
#include <cstring>

namespace ECSL
{
	typedef char* DataLocation;

	class DECLSPEC DataTable
	{
	public:
		DataTable() { }
		virtual ~DataTable() = 0;
		
		// Delete table data
		virtual void Release() = 0;
		virtual void ClearRow(unsigned const int _row) = 0;
		virtual void ClearTable() = 0;

		virtual const unsigned int GetRowCount() const = 0;

		// Get the location for all data for an id
		virtual DataLocation GetData(unsigned const int _row) = 0;
		// Get the location for a variable for an id
		virtual DataLocation GetData(unsigned const int _row, unsigned const int _index) = 0;
		// Change all data for an id
		virtual void SetData(unsigned const int _row, void* _data, unsigned const int _byteCount) = 0;
		// Change value of a variable for an id
		virtual void SetData(unsigned const int _row, unsigned const int _index, void* _data, unsigned const int _byteCount) = 0;
		
		/* Data usage */
		virtual const unsigned int GetBytesPerRow() const = 0;
		virtual const unsigned int GetMemoryAllocated() const = 0;
	};
}

#endif