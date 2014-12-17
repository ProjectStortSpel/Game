#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <SDL/SDL.h>

namespace MPL
{
	namespace MathHelper
	{
		/* Calculate how many iterations a thread should do when split between many threads */
		DECLSPEC void SplitIterations(unsigned int& _startAt, unsigned int& _endAt, unsigned int _iterations, unsigned int _taskIndex, unsigned int _taskCount);
	};
};

#endif