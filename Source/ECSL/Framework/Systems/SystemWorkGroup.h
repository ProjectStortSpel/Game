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
		
		void AddSystem(System* _system);

		void Update(float _dt);

		std::vector<System*>* GetSystems() { return m_systems; }
	};
}

#endif