#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL/SDL.h>
#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Multithreading/RuntimeInfo.h"
#include "ECSL/Framework/Systems/ComponentFilter.h"
#include "ECSL/Framework/Systems/SystemActivationManager.h"
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
		virtual void PostInitialize() { };
		virtual void Initialize() = 0;

		virtual void EntitiesAdded(const RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities) { }
		virtual void EntitiesRemoved(const RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities) { }
		virtual void MessagesReceived(const std::vector<Message*>& _messages) { }

		void InitializeBackEnd();
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
		unsigned int GetEntitiesAddedTaskCount() { return m_entitiesAddedTaskCount; }
		unsigned int GetEntitiesRemovedTaskCount() { return m_entitiesRemovedTaskCount; }
		unsigned int GetMessagesReceivedTaskCount() { return m_messagesReceivedTaskCount; }

		const std::vector<unsigned int>* GetEntities() { return m_entities; }
		const std::vector<Subscription*>* GetSubscriptions() { return m_subscriptions; }
		const std::vector<Message*>* GetMessages() { return m_messages; }

		void SetGroupId(unsigned int _groupId) { m_groupId = _groupId; }
		void SetDataManager(DataManager* _dataManager) { m_dataManager = _dataManager; }
		void SetSystemActivationManager(SystemActivationManager* _systemActivationManager) { m_systemActivationManager = _systemActivationManager; }
		void SetSystemIdManager(SystemIdManager* _idManager) { m_systemIdManager = _idManager; }

		void ComponentHasChanged(unsigned int _entityId, std::string _componentType, bool _notifyNetwork = true);
		void ComponentHasChanged(unsigned int _entityId, unsigned int _componentTypeId, bool _notifyNetwork = true);

		void ClearMessages() { m_messages->clear(); }

	protected:
		DataLocation GetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName);
		DataLocation GetComponent(unsigned int _entityId, const std::string& _componentType, unsigned int _index);
		DataLocation GetComponent(unsigned int _entityId, unsigned int _componentTypeId, unsigned int _index);
		void GetComponents(std::vector<DataLocation>& _out, const std::vector<unsigned int>* _entityIds, const std::string& _componentType, const std::string& _variableName);
		void GetComponents(std::vector<DataLocation>& _out, const std::vector<unsigned int>* _entityIds, const std::string& _componentType, unsigned int _index);
		void GetComponents(std::vector<DataLocation>& _out, const std::vector<unsigned int>* _entityIds, unsigned int _componentTypeId, unsigned int _index);

		void SetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName, void* _data);
		void SetComponent(unsigned int _entityId, unsigned int _componentTypeId, unsigned int _index, void* _data, unsigned int _byteSize);

		bool HasComponent(unsigned int _entityId, const std::string& _componentType);
		bool HasComponent(unsigned int _entityId, unsigned int _componentTypeId);

		void CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId);
		void CreateComponentAndAddTo(unsigned int _componentTypeId, unsigned int _entityId);
		void RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId);
		void RemoveComponentFrom(unsigned int _componentTypeId, unsigned int _entityId);

		ComponentTable* GetComponentTable(const std::string& _componentType);
		ComponentTable* GetComponentTable(unsigned int _componentTypeId);
		unsigned int GetComponentVariableIndex(const std::string& _componentType, const std::string& _variableName);

		unsigned int CreateNewEntity();
		void KillEntity(unsigned int _entityId);

		const unsigned int GetEntityCount() { return m_dataManager->GetEntityCount(); }
		const unsigned int GetThreadCount() { return MPL::TaskManager::GetInstance().GetThreadCount(); }
		
		void AddComponentTypeToFilter(const std::string& _componentType, FilterType _filterType);
		void SetSystemName(const std::string& _name);
		void SetUpdateTaskCount(unsigned int _taskCount);
		void SetEntitiesAddedTaskCount(unsigned int _taskCount);
		void SetEntitiesRemovedTaskCount(unsigned int _taskCount);
		void SetMessagesReceivedTaskCount(unsigned int _taskCount);

		void SubscribeTo(const std::string& _systemName, unsigned int _messageType);
		void SendMessage(Message* _message);

		void ActivateSystem(const std::string& _systemName);
		void DeactivateSystem(const std::string& _systemName);

		/* Writes all entity, component and system data to log */
		void LogWorldData();

	private:
		bool m_initialized;
		unsigned int m_id;
		unsigned int m_groupId;
		std::string* m_systemName;
		unsigned int m_updateTaskCount;
		unsigned int m_entitiesAddedTaskCount;
		unsigned int m_entitiesRemovedTaskCount;
		unsigned int m_messagesReceivedTaskCount;
		
		SDL_mutex* m_messagesMutex;
		ComponentFilter m_mandatoryComponentTypes;
		ComponentFilter m_requiresOneOfComponentTypes;
		ComponentFilter m_excludedComponentTypes;
		BitSet::DataType* m_entitiesBitSet;
		std::vector<unsigned int>* m_entities;
		std::vector<Subscription*>* m_subscriptions;
		std::vector<Message*>* m_messages;
		DataManager* m_dataManager;
		SystemActivationManager* m_systemActivationManager;
		SystemIdManager* m_systemIdManager;
	};
}

#endif