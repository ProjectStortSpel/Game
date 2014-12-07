#ifndef TASKINFO_H
#define TASKINFO_H

#include <SDL/SDL.h>

namespace MPL
{
	class DECLSPEC TaskInfo
	{
	public:
		TaskInfo(unsigned int _taskId, unsigned int _threadId);
		~TaskInfo();

		unsigned int GetTaskId() { return m_taskId; }
		unsigned int GetThreadId() { return m_threadId; }
		float GetElapsedSeconds() { return m_dt; }

	private:
		unsigned int m_taskId;
		unsigned int m_threadId;
		float m_dt;
	};
};

#endif