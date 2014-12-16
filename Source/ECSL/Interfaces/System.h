#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL/SDL.h>
#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Systems/ComponentFilter.h"
#include "ECSL/Framework/Systems/SystemIdManager.h"
#include "ECSL/Framework/Systems/Messaging/Message.h"
#include "MPL/Framework/Tasks/Task.h"

namespace ECSL
{
	class DECLSPEC System
	{
	public:
		System();
		virtual ~System() = 0;

		virtual void Update(unsigned int _taskIndex, float _dt) { }
		virtual void Initialize() = 0;

		virtual void OnEntityAdded(unsigned int _taskIndex, float _dt, unsigned int _entityId) { }
		virtual void OnEntityRemoved(unsigned int _taskIndex, float _dt, unsigned int _entityId) { }

		virtual void OnMessageRecieved(const System* _sender, const Message* _message) { }

		void InitializeEntityList();
		void AddEntityToSystem(unsigned int _entityId);
		void RemoveEntityFromSystem(unsigned int _entityId);
		bool HasEntity(unsigned int _entityId);

		ComponentFilter* GetMandatoryFilter() { return &m_mandatoryComponentTypes; }
		ComponentFilter* GetRequiresOneOfFilter() { return &m_requiresOneOfComponentTypes; }
		ComponentFilter* GetExcludedFilter() { return &m_excludedComponentTypes; }

		unsigned int GetId() { return m_id; }
		unsigned int GetGroupId() { return m_groupId; }
		const std::string& GetSystemName() { return *m_systemName; }

		unsigned int GetUpdateTaskCount() { return m_updateTaskCount; }
		unsigned int GetOnEntityAddedTaskCount() { return m_onEntityAddedTaskCount; }
		unsigned int GetOnEntityRemovedTaskCount() { return m_onEntityRemovedTaskCount; }

		void SetId(unsigned int _id) { m_id = _id; }
		void SetGroupId(unsigned int _groupId) { m_groupId = _groupId; }
		void SetDataManager(DataManager* _dataManager) { m_dataManager = _dataManager; }
		void SetSystemIdManager(SystemIdManager* _idManager) { m_systemIdManager = _idManager; }

	protected:
		DataLocation GetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName);
		DataLocation GetComponent(unsigned int _entityId, const std::string& _componentType, unsigned int _index);
		DataLocation GetComponent(unsigned int _entityId, unsigned int _componentTypeId, unsigned int _index);

		void SetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName, void* _data);
		void SetComponent(unsigned int _entityId, unsigned int _componentTypeId, unsigned int _index, void* _data, unsigned int _byteSize);

		void CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId);
		void CreateComponentAndAddTo(unsigned int _componentTypeId, unsigned int _entityId);
		void RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId);
		void RemoveComponentFrom(unsigned int _componentTypeId, unsigned int _entityId);

		ComponentTable* GetComponentTable(const std::string& _componentType);
		ComponentTable* GetComponentTable(unsigned int _componentTypeId);
		unsigned int GetComponentVariableIndex(const std::string& _componentType, const std::string& _variableName);

		unsigned int CreateNewEntity();
		void KillEntity(unsigned int _entityId);
		const std::vector<unsigned int>* const GetEntities() { return m_entities; }

		template<typename SystemType>
		void SendMessage(unsigned int _messageType, Message* _message);
		void SendMessage(unsigned int _messageType, Message* _message);

		void AddComponentTypeToFilter(const std::string& _componentType, FilterType _filterType);
		void SetSystemName(const std::string& _name) { *m_systemName = _name; }
		void SetUpdateTaskCount(unsigned int _taskCount) { m_updateTaskCount = _taskCount; }
		void SetOnEntityAddedTaskCount(unsigned int _taskCount) { m_onEntityAddedTaskCount = _taskCount; }
		void SetOnEntityRemovedTaskCount(unsigned int _taskCount) { m_onEntityRemovedTaskCount = _taskCount; }
		void SetOnMessageRecievedTaskCount(unsigned int _taskCount) { m_onMessageRecievedTaskCount = _taskCount; }
		void SetOnEventTaskCount(unsigned int _workCount, void* _onEventFunction);

		//template<typename SystemType>
		//void SubscribeTo<SystemType>(unsigned int _messageType, void* _functionPointer);
		//void SubscribeTo(const std::string& _systemName, unsigned int _messageType);
		//template<typename SystemType>
		//void UnsubscribeFrom<SystemType>(unsigned int _messageType);
		//void UnsubscribeFrom(const std::string& _systemName, unsigned int _messageType);

	private:
		unsigned int m_id;
		unsigned int m_groupId;
		std::string* m_systemName;
		unsigned int m_updateTaskCount;
		unsigned int m_onEntityRemovedTaskCount;
		unsigned int m_onEntityAddedTaskCount;
		unsigned int m_onMessageRecievedTaskCount;
		
		ComponentFilter m_mandatoryComponentTypes;
		ComponentFilter m_requiresOneOfComponentTypes;
		ComponentFilter m_excludedComponentTypes;
		BitSet::DataType* m_entitiesBitSet;
		std::vector<unsigned int>* m_entities;
		DataManager* m_dataManager;
		SystemIdManager* m_systemIdManager;
	};
}

#endif