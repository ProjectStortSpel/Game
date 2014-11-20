#ifndef SYSTEMWORKGROUP_H
#define SYSTEMWORKGROUP_H

#include <SDL/SDL.h>
#include "../../Interfaces/System.h"

namespace ECSL
{
	// Each system in a group runs concurrently on different threads
	class DECLSPEC SystemWorkGroup
	{
	private:
		std::vector<System*>* m_systems;
	public:
		SystemWorkGroup();
		explicit SystemWorkGroup(System* _system);
		~SystemWorkGroup();
		
		template<typename SystemType>
		void AddSystem();
	};

	template<typename SystemType>
	void SystemWorkGroup::AddSystem()
	{
		m_systems->push_back(new SystemType());
	}
}

#endif