#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <SDL/SDL.h>
#include <vector>
#include "SystemWorkGroup.h"

namespace ECSL
{
	class DECLSPEC SystemManager
	{
	private:
		std::vector<SystemWorkGroup*>* m_systemWorkGroups;

		void GenerateSystemBitmask(System* _system, ComponentFilter _type);

	public:
		explicit SystemManager(std::vector<SystemWorkGroup*>* _systemWorkGroups);
		~SystemManager();

		void InitializeSystems();
		void Update(float _dt);
	};
}

#endif