#ifndef WORLD_H
#define WORLD_H

#include <SDL/SDL.h>
#include <string>

#include "Simulation.h"
#include "Components/DataManager.h"
#include "Systems/SystemWorkGroup.h"
#include "Systems/SystemManager.h"

namespace ECSL
{
	class DECLSPEC World
	{
	private:
		Simulation* m_simulation;
		DataManager* m_dataManager;
		SystemManager* m_systemManager;

	public:
		World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<unsigned int>* _componentTypeIds);
		~World();

		void Update(float _dt);

		unsigned int CreateNewEntity();
		void CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId);
		void RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId);
		template<typename ReturnType>
		const ReturnType* const GetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName);
		void SetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName, void* _data);
		void KillEntity(unsigned int _entityId);
	};

	template<typename ReturnType>
	const ReturnType* const World::GetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName)
	{
		ComponentTable* componentTable = m_dataManager->GetComponentTable(ComponentTypeManager::GetInstance().GetTableId(_componentType));
		return (ReturnType*)componentTable->GetComponent(_entityId, _variableName);
	}
}

#endif