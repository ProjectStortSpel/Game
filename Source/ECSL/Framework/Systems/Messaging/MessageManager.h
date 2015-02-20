#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <SDL/SDL.h>
#include <unordered_map>
#include <vector>
#include <deque>
#include "Subscription.h"
#include "ECSL/Framework/Systems/SystemManager.h"

namespace ECSL
{
	class DECLSPEC MessageManager
	{
	public:
		MessageManager(SystemManager* _systemManager);
		~MessageManager();

		void Initialize();

		void SortMessages(const RuntimeInfo& _runtime);
		void DeleteMessages(const RuntimeInfo& _runtime);

		const std::vector<Message*>* GetMessagesToSystem(unsigned int _systemId) { return (*m_messagesToSystems)[_systemId]; }

	private:
		SystemManager* m_systemManager;
		std::vector<System*>* m_subscribers;
		std::unordered_map<unsigned int, std::vector<Message*>*>* m_messagesToSystems;
	};
}

#endif