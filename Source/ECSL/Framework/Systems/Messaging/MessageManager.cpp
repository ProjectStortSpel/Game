#include "MessageManager.h"

using namespace ECSL;

MessageManager::MessageManager()
: m_systemSubscriptions(new std::map<unsigned int, std::map<unsigned int, std::vector<Subscription>>>)
{

}

MessageManager::~MessageManager()
{

}

void MessageManager::AddSubscription(unsigned int _subscriberId, unsigned int _senderId, unsigned int _messageType, void* _onEventFunction)
{
	(*m_systemSubscriptions)[_senderId][_messageType].push_back(Subscription(_subscriberId, _senderId, _messageType, _onEventFunction));
}

void MessageManager::RemoveSubscription(unsigned int _subscriberId, unsigned int _senderId, unsigned int _messageType)
{
	std::vector<Subscription>* subscriptions = &((*m_systemSubscriptions)[_senderId][_messageType]);
	for (auto it = subscriptions->begin(); it != subscriptions->end(); ++it)
		if (it->GetSubscriberId() == _subscriberId)
		{
			subscriptions->erase(it);
			return;
		}
}