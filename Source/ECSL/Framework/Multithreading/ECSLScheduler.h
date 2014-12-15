#ifndef ECSLSCHEDULER_H
#define ECSLSCHEDULER_H

#include <vector>
#include "ECSL/Interfaces/System.h"
#include "MPL/Framework/Tasks/Task.h"
#include "MPL/Interfaces/Scheduler.h"
//
//namespace ECSL
//{
//	typedef void (System::*UpdatePointer)(unsigned int, float);
//
//	class DECLSPEC ECSLScheduler : public MPL::Scheduler
//	{
//	public:
//		ECSLScheduler();
//		~ECSLScheduler();
//
//		void Execute();
//
//		void AddSystemGroupUpdate(std::vector<System*>* m_systems);
//		void AddSystemGroupOnEntityAdded(std::vector<System*>* m_systems);
//		void AddSystemGroupOnEntityRemoved(std::vector<System*>* m_systems);
//
//		
//		void SetElapsedTime(float _dt) { m_dt = _dt; }
//
//	private:
//		float m_dt;
//	};
//
//	static void SystemUpdate(const MPL::TaskInfo& _taskInfo, void* _data);
//
//};

#endif