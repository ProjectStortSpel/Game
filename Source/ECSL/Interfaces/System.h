#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL/SDL.h>
#include <map>
#include <vector>

namespace ECSL
{
	class DECLSPEC System
	{
	private:
		bool m_initialized;
		unsigned int m_workCount;
		//std::vector<int>* m_componentTypeIds;

		//World* m_world;
	protected:
		

	public:
		System() { }
		virtual ~System() = 0 { }

		virtual void Run() = 0;
		virtual void Initialize() = 0;

		virtual void OnEntityAdded(unsigned int _entityId) = 0;
		virtual void OnEntityRemoved(unsigned int _entityId) = 0;
	};
}

#endif