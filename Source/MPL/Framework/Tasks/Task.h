#ifndef TASK_H
#define TASK_H

#include <SDL/SDL.h>
#include "TaskInfo.h"

namespace MPL
{
	typedef void(*FunctionPointer)(const TaskInfo&, void*);

	class DECLSPEC Task
	{
	public:
		Task(FunctionPointer _function);
		Task(FunctionPointer _function, void* _data);
		~Task();

		FunctionPointer GetFunction() { return m_function; }
		void* GetData() { return m_data; }
		void SetData(void* _data) { m_data = _data; }

	private:
		FunctionPointer m_function;
		void* m_data;
	};
};

#endif