#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <SDL/SDL.h>
#include <map>
#include <vector>
#include "Subscription.h"
#include "ECSL/Framework/Systems/SystemIdManager.h"

namespace ECSL
{
	class DECLSPEC MessageManager
	{
	public:
		MessageManager();
		~MessageManager();

		template<typename Sender>
		void AddSubscription(System* _subscriber, unsigned int _messageType, void* _onEventFunction);
		void AddSubscription(const std::string& _sender, System* _subscriber, unsigned int _messageType, void* _onEventFunction);

		template<typename Sender>
		void RemoveSubscription(System* _subscriber, unsigned int _messageType);
		void RemoveSubscription(const std::string& _sender, System* _subscriber, unsigned int _messageType);

		void GatherMessages();
		void SendMessages();
		void DeleteMessages();

	private:
		/* [SystemId][MessageType]->Subscription */
		std::map<unsigned int, std::map<unsigned int, std::vector<Subscription>>>* m_systemSubscriptions;
	};

	template<typename Sender>
	void MessageManager::AddSubscription(System* _subscriber, unsigned int _messageType, void* _onEventFunction)
	{
		(*m_systemSubscriptions)[SystemIdManager::GetInstance().GetSystemId<Sender>()][_messageType].push_back(Subscription(_subscriber, _onEventFunction));
	}

	template<typename Sender>
	void MessageManager::RemoveSubscription(System* _subscriber, unsigned int _messageType)
	{
		//std::vector<Subscription>* subscriptions = &((*m_systemSubscriptions)[SystemIdManager::GetInstance().GetSystemId<Sender>()][_messageType]));
		//for (auto it = subscriptions->begin(); it != subscriptions->end(); ++it)
		//	if (it->GetSubscriber() == _subscriber)
		//	{
		//		subscriptions->erase(it);
		//		return;
		//	}
	}
}

#endif