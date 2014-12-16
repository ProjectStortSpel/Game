#ifndef TASK_H
#define TASK_H

#include <SDL/SDL.h>
#include "TaskId.h"
#include "WorkItem.h"

namespace MPL
{
	struct Task
	{
		TaskId Id;
		WorkItem* Work;
		unsigned int OpenWorkItemCount;
		TaskId Dependency;
	};
};

#endif