#ifndef DATATABLE_H
#define DATATABLE_H

#include <SDL/SDL.h>

namespace ECSL
{
	typedef const char* const DataLocation;

	class DECLSPEC DataTable
	{
	private:
		
	public:
		DataTable() { }
		virtual ~DataTable() = 0;
		
		// Delete table data
		virtual void Release() = 0;
		virtual void ClearRow(unsigned const int _row) = 0;
		virtual void ClearTable() = 0;

		virtual const int GetRowCount() const = 0;

		// Get the location for all data for an id
		virtual DataLocation GetData(unsigned const int _id) const = 0;
		// Get the location for a variable for an id
		virtual DataLocation GetData(unsigned const int _id, unsigned const int _index) const = 0;
		// Change all data for an id
		virtual void SetData(unsigned const int _id, void* _data, unsigned const int _byteCount) = 0;
		// Change value of a variable for an id
		virtual void SetData(unsigned const int _id, unsigned const int _index, void* _data, unsigned const int _byteCount) = 0;
		
		/* Data usage */
		virtual const int GetBytesPerRow() const = 0;
		virtual const int GetMemoryAllocated() const = 0;
	};
}

#endif