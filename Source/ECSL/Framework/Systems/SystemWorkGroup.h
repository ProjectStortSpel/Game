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
		void AddSystem(System* _system);

		void Update(float _dt);

		std::vector<System*>* GetSystems() { return m_systems; }
	};

	template<typename SystemType>
	void SystemWorkGroup::AddSystem()
	{
		/*	TODO: Add check so same system doesn't get added	*/
		m_systems->push_back(new SystemType());
	}
}

#endif