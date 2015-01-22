#ifndef WORKITEM_H
#define WORKITEM_H

#include <SDL/SDL.h>
#include <string>

namespace MPL
{
	typedef void(*FunctionPointer)(void*);

	struct DECLSPEC WorkItem
	{
		~WorkItem() { delete(ProfilerName); }

		FunctionPointer Work;
		void* Data;
		std::string* ProfilerName;
		unsigned int ProfilerGroupId;
	};
};

#endif