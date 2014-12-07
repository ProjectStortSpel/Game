#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <SDL/SDL.h>
#include <map>
#include <vector>
#include "Subscription.h"

namespace ECSL
{
	class DECLSPEC MessageManager
	{
	public:
		MessageManager();
		~MessageManager();

		void AddSubscription(unsigned int _subscriberId, unsigned int _senderId, unsigned int _messageType, void* _onEventFunction);
		void RemoveSubscription(unsigned int _subscriberId, unsigned int _senderId, unsigned int _messageType);

		void GatherMessages();
		void SendMessages();
		void DeleteMessages();

	private:
		/* m_systemSubscriptions[SenderId][MessageType].Subscription */
		std::map<unsigned int, std::map<unsigned int, std::vector<Subscription>>>* m_systemSubscriptions;
	};
}

#endif