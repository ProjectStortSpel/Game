#ifndef TASK_H
#define TASK_H

#include <SDL/SDL.h>
#include "TaskId.h"
#include "TaskInfo.h"
#include "WorkItem.h"

namespace MPL
{
	class Task
	{
	public:
		Task(WorkItem _function);
		~Task();

	private:
		TaskId m_taskId;
		WorkItem* m_work;
		TaskId m_parentId;
		unsigned int m_openWorkItemCount;
		TaskId m_dependency;
	};
};

#endif