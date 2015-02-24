#ifndef RUNTIMEINFO_H
#define RUNTIMEINFO_H

#include <SDL/SDL.h>
#include <MPL/Framework/Common/MathHelper.h>

namespace ECSL
{
	struct DECLSPEC RuntimeInfo
	{
		unsigned int TaskIndex;
		unsigned int TaskCount;
		float Dt;
	};
};

#endif