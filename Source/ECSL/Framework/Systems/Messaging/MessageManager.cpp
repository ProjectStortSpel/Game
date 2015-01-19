#include "MessageManager.h"

#include "ECSL/Framework/Multithreading/RuntimeInfo.h"

using namespace ECSL;

MessageManager::MessageManager(SystemManager* _systemManager)
:	m_systemManager(_systemManager),
	m_subscribers(new std::vector<System*>()),
	m_messagesToSystems(new std::unordered_map<unsigned int, std::vector<Message*>*>())
{

}

MessageManager::~MessageManager()
{
	if (m_subscribers)
		delete(m_subscribers);
	if (m_messagesToSystems)
	{
		RuntimeInfo runtime = { runtime.TaskIndex = 0, runtime.TaskCount = 1 };
		DeleteMessages(runtime);
		for (unsigned int i = 0; i < m_messagesToSystems->size(); ++i)
			delete(m_messagesToSystems->at(i));
		delete(m_messagesToSystems);
	}
}

void MessageManager::Initialize()
{
	for (auto system : *m_systemManager->GetSystems())
	{
		if (system->GetSubscriptions()->size() > 0)
		{
			for (auto subscription : *system->GetSubscriptions())
			{
				subscription->SetSubscriberId(m_systemManager->GetSystemIdManager()->GetSystemId(subscription->GetSubscriberName()));
				subscription->SetSubscribesToId(m_systemManager->GetSystemIdManager()->GetSystemId(subscription->GetSubscribesToName()));
			}
			m_subscribers->push_back(system);
			(*m_messagesToSystems)[system->GetId()] = new std::vector<Message*>();
		}
	}
}

void MessageManager::SortMessages(const RuntimeInfo& _runtime)
{
	const std::vector<System*>* systems = m_systemManager->GetSystems();
	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, m_subscribers->size(), _runtime.TaskIndex, _runtime.TaskCount);
	/* Loop through each subscriber system */
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		System* subscriber = m_subscribers->at(i);
		/* Loop through each system subscription */
		for (auto subscription : *subscriber->GetSubscriptions())
		{
			System* sender = systems->at(subscription->GetSubscribesToId());
			/* Loop through each senders' messages */
			for (auto message : *sender->GetMessages())
			{
				/* Ignore messages that doesn't match the message type */
				if (message->GetMessageType() != subscription->GetMessageType())
					continue;
				/* Store message pointer in list. Message will be sent to subscribers in the function SendMessages() */
				(*m_messagesToSystems)[subscriber->GetId()]->push_back(message);
			}
		}
	}
}

void MessageManager::DeleteMessages(const RuntimeInfo& _runtime)
{
	const std::vector<System*>* systems = m_systemManager->GetSystems();
	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, systems->size(), _runtime.TaskIndex, _runtime.TaskCount);
	/* Loop through each subscriber system */
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		System* system = systems->at(i);
		for (unsigned int messageIndex = 0; messageIndex < system->GetMessages()->size(); ++messageIndex)
		{
			delete(system->GetMessages()->at(messageIndex));
		}
		system->ClearMessages();
	}

	for (auto messages : *m_messagesToSystems)
		messages.second->clear();
}