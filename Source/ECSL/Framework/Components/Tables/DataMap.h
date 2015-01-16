#ifndef DATAMAP_H
#define DATAMAP_H

#include <SDL/SDL.h>
#include <map>
#include "DataTable.h"

namespace ECSL
{
	class DECLSPEC DataMap : public DataTable
	{
	public:
		DataMap(unsigned const int _bytesPerRow);
		~DataMap();

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
		std::map<unsigned int, char*>* m_dataMap;
		unsigned int m_bytesPerRow;
	};
}
#endif