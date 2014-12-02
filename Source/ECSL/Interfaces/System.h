#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL/SDL.h>
#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Systems/ComponentFilter.h"

namespace ECSL
{
	enum SystemType
	{
		Both = 0,
		Listener = 1,
		Updater
	};

	class DECLSPEC System
	{
	public:
		System();
		virtual ~System() = 0;

		virtual void Run() = 0;
		virtual void Initialize() = 0;

		virtual void OnEntityAdded(unsigned int _entityId) = 0;
		virtual void OnEntityRemoved(unsigned int _entityId) = 0;

		void SetDataManager(DataManager* _dataManager) { m_dataManager = _dataManager; }

		void InitializeEntityList();
		void AddEntity(unsigned int _entityId);
		void RemoveEntity(unsigned int _entityId);
		bool HasEntity(unsigned int _entityId);

		ComponentFilter* GetMandatoryFilter() { return &m_mandatoryComponentTypes; }
		ComponentFilter* GetRequiresOneOfFilter() { return &m_requiresOneOfComponentTypes; }
		ComponentFilter* GetExcludedFilter() { return &m_excludedComponentTypes; }

		const SystemType GetSystemType() const { return m_systemType; }

	protected:
		void AddComponentTypeToFilter(const std::string& _componentType, FilterType _filterType);
		void SetSystemType(SystemType _systemType) { m_systemType = _systemType; }

	private:
		bool m_initialized;
		unsigned int m_workCount;
		SystemType m_systemType;
		ComponentFilter m_mandatoryComponentTypes;
		ComponentFilter m_requiresOneOfComponentTypes;
		ComponentFilter m_excludedComponentTypes;
		BitSet::DataType* m_entities;
		DataManager* m_dataManager;
	};
}

#endif