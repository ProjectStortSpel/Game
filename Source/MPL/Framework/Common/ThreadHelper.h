#ifndef THREADHELPER_H
#define THREADHELPER_H

#include <SDL/SDL.h>

namespace MPL
{
	namespace ThreadHelper
	{
		/* Returns the thread count of the current process or -1 in case of failure. */
		int GetCurrentThreadCount();
	};
};

#endif