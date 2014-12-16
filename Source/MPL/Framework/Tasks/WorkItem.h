#ifndef WORKITEM_H
#define WORKITEM_H

#include <SDL/SDL.h>

namespace MPL
{
	typedef void(*FunctionPointer)(void*);

	struct DECLSPEC WorkItem
	{
		FunctionPointer Work;
		void* Data;
	};
};

#endif