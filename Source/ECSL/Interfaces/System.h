#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL/SDL.h>
#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Multithreading/RuntimeInfo.h"
#include "ECSL/Framework/Systems/ComponentFilter.h"
#include "ECSL/Framework/Systems/SystemIdManager.h"
#include "ECSL/Framework/Systems/Messaging/Message.h"
#include "ECSL/Framework/Systems/Messaging/Subscription.h"
#include "MPL/Framework/Tasks/Task.h"
#include "MPL/Managers/TaskManager.h"

namespace ECSL
{
	class DECLSPEC System
	{
	public:
		System();
		virtual ~System() = 0;

		virtual void Update(const RuntimeInfo& _runtime) { }
		virtual void Initialize() = 0;

		virtual void EntitiesAdded(const RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities) { }
		virtual void EntitiesRemoved(const RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities) { }
		virtual void MessagesRecieved(const std::vector<Message*>& _messages) { }

		void InitializeEntityList();
		void AddEntityToSystem(unsigned int _entityId);
		void RemoveEntityFromSystem(unsigned int _entityId);
		bool HasEntity(unsigned int _entityId);
		void ComponentHasChanged(unsigned int _entityId, std::string _componentType);
		void ComponentHasChanged(unsigned int _entityId, unsigned int _componentTypeId);

		ComponentFilter* GetMandatoryFilter() { return &m_mandatoryComponentTypes; }
		ComponentFilter* GetRequiresOneOfFilter() { return &m_requiresOneOfComponentTypes; }
		ComponentFilter* GetExcludedFilter() { return &m_excludedComponentTypes; }

		unsigned int GetId() { return m_id; }
		unsigned int GetGroupId() { return m_groupId; }
		const std::string& GetSystemName() { return *m_systemName; }

		unsigned int GetUpdateTaskCount() { return m_updateTaskCount; }
		unsigned int GetEntitiesAddedTaskCount() { return m_entitiesAddedTaskCount; }
		unsigned int GetEntitiesRemovedTaskCount() { return m_entitiesRemovedTaskCount; }
		unsigned int GetMessagesRecievedTaskCount() { return m_messagesRecievedTaskCount; }

		const std::vector<Subscription*>* GetSubscriptions() { return m_subscriptions; }
		const std::vector<Message*>* GetMessages() { return m_messages; }

		void SetGroupId(unsigned int _groupId) { m_groupId = _groupId; }
		void SetDataManager(DataManager* _dataManager) { m_dataManager = _dataManager; }
		void SetSystemIdManager(SystemIdManager* _idManager) { m_systemIdManager = _idManager; }

		void ClearMessages() { m_messages->clear(); }

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

		const unsigned int GetThreadCount() { return MPL::TaskManager::GetInstance().GetThreadCount(); }

		inline unsigned int GetEntityCountLimit(){ return m_dataManager->GetEntityCountLimit(); }
		inline bool EntityHasComponent(unsigned int _entityId, unsigned int _componentTypeId){ return m_dataManager->EntityHasComponent(_entityId, _componentTypeId); }
		void AddComponentTypeToFilter(const std::string& _componentType, FilterType _filterType);
		void SetSystemName(const std::string& _name) { *m_systemName = _name; m_id = m_systemIdManager->CreateSystemId(_name); }
		void SetUpdateTaskCount(unsigned int _taskCount) { m_updateTaskCount = _taskCount; }
		void SetEntitiesAddedTaskCount(unsigned int _taskCount) { m_entitiesAddedTaskCount = _taskCount; }
		void SetEntitiesRemovedTaskCount(unsigned int _taskCount) { m_entitiesRemovedTaskCount = _taskCount; }
		void SetMessagesRecievedTaskCount(unsigned int _taskCount) { m_messagesRecievedTaskCount = _taskCount; }
		void SubscribeTo(const std::string& _systemName, unsigned int _messageType);
		void SendMessage(Message* _message);

	private:
		unsigned int m_id;
		unsigned int m_groupId;
		std::string* m_systemName;
		unsigned int m_updateTaskCount;
		unsigned int m_entitiesAddedTaskCount;
		unsigned int m_entitiesRemovedTaskCount;
		unsigned int m_messagesRecievedTaskCount;
		
		SDL_mutex* m_messagesMutex;
		ComponentFilter m_mandatoryComponentTypes;
		ComponentFilter m_requiresOneOfComponentTypes;
		ComponentFilter m_excludedComponentTypes;
		BitSet::DataType* m_entitiesBitSet;
		std::vector<unsigned int>* m_entities;
		std::vector<Subscription*>* m_subscriptions;
		std::vector<Message*>* m_messages;
		DataManager* m_dataManager;
		SystemIdManager* m_systemIdManager;
	};
}

#endif