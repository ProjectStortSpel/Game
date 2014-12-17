#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "RuntimeInfo.h"
#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Systems/SystemWorkGroup.h"
#include "MPL/Framework/Tasks/Task.h"

namespace ECSL
{
	struct UpdateData
	{
		System* System;
		RuntimeInfo RuntimeInfo;
	};

	struct OnEntityData
	{
		System* System;
		RuntimeInfo RuntimeInfo;
		unsigned int EntityId;
	};

	class DECLSPEC Scheduler
	{
	public:
		Scheduler(DataManager* _dataManager);
		~Scheduler();

		void UpdateDt(float _dt);
		void ScheduleUpdate(float _dt);

		void AddUpdateGroup(const std::vector<System*>& _systems);
		void AddOnEntityAddedGroup(const std::vector<System*>& _systems);
		void AddOnEntityRemovedGroup(const std::vector<System*>& _systems);
		void AddDataManagerTasks();

	private:
		DataManager* m_dataManager;
		std::vector<std::vector<MPL::WorkItem*>*>* m_updateWorkItems;
	};

	static void SystemUpdate(void* _data);

	static void SystemOnEntityAdded(void* _data);

	static void SystemOnEntityRemoved(void* _data);

};

#endif