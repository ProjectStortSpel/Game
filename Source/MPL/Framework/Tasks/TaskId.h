#ifndef TASKID_H
#define TASKID_H

#include <SDL/SDL.h>

namespace MPL
{
	typedef long long TaskId;
	const TaskId NoDependency = -1;
};

#endif