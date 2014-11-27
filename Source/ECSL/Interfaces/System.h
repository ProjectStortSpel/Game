#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL/SDL.h>
#include "ECSL/Framework/Systems/SystemBitmask.h"
#include <map>
#include <vector>

namespace ECSL
{
	class DECLSPEC System
	{
	private:
		bool m_initialized;
		unsigned int m_workCount;

		SystemBitmask m_mandatoryComponentTypes;
		SystemBitmask m_requiresOneOfComponentTypes;
		SystemBitmask m_excludedComponentTypes;
		//std::vector<int>* m_componentTypeIds;

		//World* m_world;
	protected:
		void AddComponentTypeToFilter(std::string _componentType, ComponentFilter _filter);
		

	public:
		System() { }
		virtual ~System() { }

		virtual void Run() = 0;
		virtual void Initialize() = 0;

		virtual void OnEntityAdded(unsigned int _entityId) = 0;
		virtual void OnEntityRemoved(unsigned int _entityId) = 0;

		SystemBitmask* GetSystemBitmask(ComponentFilter _bitmaskType);
	};
}

#endif