#ifndef TASK_H
#define TASK_H

#include <SDL/SDL.h>
#include "TaskInfo.h"

namespace MPL
{
	class DECLSPEC Task
	{
	public:
		Task(unsigned int _workCount, void (*_function)(const MPL::TaskInfo&, void*));
		Task(unsigned int _workCount, void* _function, void* _data);
		~Task();

		void* GetFunction() { return m_function; }
		void* GetData() { return m_data; }
		void SetData(void* _data) { m_data = _data; }

	private:
		void* m_function;
		void* m_data;
	};
};

#endif