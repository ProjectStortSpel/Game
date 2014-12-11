#include "Task.h"

using namespace MPL;

Task::Task(void(*_function)(const MPL::TaskInfo&, void*))
: m_function(_function), m_data(0)
{

}

Task::Task(void(*_function)(const MPL::TaskInfo&, void*), void* _data)
: m_function(_function), m_data(_data)
{

}

Task::~Task()
{
	
}