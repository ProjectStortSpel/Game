#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL/SDL.h>
#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Systems/ComponentFilter.h"

namespace ECSL
{
	class DECLSPEC System
	{
	public:
		System();
		virtual ~System() = 0;

		virtual void Update(float _dt) = 0;
		virtual void Initialize() = 0;

		virtual void OnEntityAdded(unsigned int _entityId) = 0;
		virtual void OnEntityRemoved(unsigned int _entityId) = 0;

		void SetDataManager(DataManager* _dataManager) { m_dataManager = _dataManager; }

		void InitializeEntityList();
		void AddEntityToSystem(unsigned int _entityId);
		void RemoveEntityFromSystem(unsigned int _entityId);
		bool HasEntity(unsigned int _entityId);

		ComponentFilter* GetMandatoryFilter() { return &m_mandatoryComponentTypes; }
		ComponentFilter* GetRequiresOneOfFilter() { return &m_requiresOneOfComponentTypes; }
		ComponentFilter* GetExcludedFilter() { return &m_excludedComponentTypes; }

		const std::string& GetSystemName() { return m_systemName; }

		void ComponentHasChanged(unsigned int _entityId, std::string _componentType);
		
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
		void AddComponentTypeToFilter(const std::string& _componentType, FilterType _filterType);

		unsigned int CreateNewEntity();
		void KillEntity(unsigned int _entityId);
		const std::vector<unsigned int>* const GetEntities() { return m_entities; }

		void SetSystemName(std::string _name){ m_systemName = _name; }

	private:
		bool m_initialized;
		unsigned int m_workCount;
		std::string m_systemName;
		ComponentFilter m_mandatoryComponentTypes;
		ComponentFilter m_requiresOneOfComponentTypes;
		ComponentFilter m_excludedComponentTypes;
		BitSet::DataType* m_entitiesBitSet;
		std::vector<unsigned int>* m_entities;
		DataManager* m_dataManager;
	};
}

#endif