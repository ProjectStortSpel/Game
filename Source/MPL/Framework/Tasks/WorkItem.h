#ifndef WORKITEM_H
#define WORKITEM_H

#include <SDL/SDL.h>
#include <string>

namespace MPL
{
	typedef void(*FunctionPointer)(void*);

	struct DECLSPEC WorkItem
	{
		~WorkItem() { delete(Name); }

		FunctionPointer Work;
		void* Data;
		std::string* Name;
		unsigned int LocalGroupId;
		unsigned int GroupId;
	};
};

#endif