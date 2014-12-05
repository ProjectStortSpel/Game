#include "MessageManager.h"

using namespace ECSL;

MessageManager::MessageManager()
: m_systemSubscriptions(new std::map<unsigned int, std::map<unsigned int, std::vector<Subscription>>>)
{

}

MessageManager::~MessageManager()
{

}

void MessageManager::AddSubscription(const std::string& _sender, System* _subscriber, unsigned int _messageType, void* _onEventFunction)
{
	(*m_systemSubscriptions)[SystemIdManager::GetInstance().GetSystemId(_sender)][_messageType].push_back(Subscription(_subscriber, _onEventFunction));
}

void MessageManager::RemoveSubscription(const std::string& _sender, System* _subscriber, unsigned int _messageType)
{
	//std::vector<Subscription>* subscriptions = &((*m_systemSubscriptions)[SystemIdManager::GetInstance().GetSystemId(_sender)][_messageType]);
	//for (auto it = subscriptions->begin(); it != subscriptions->end(); ++it)
	//	if (it->GetSubscriber() == _subscriber)
	//	{
	//		subscriptions->erase(it);
	//		return;
	//	}
}

void SendMessage(System* _sender, unsigned int _messageType, Message* _message)
{

}