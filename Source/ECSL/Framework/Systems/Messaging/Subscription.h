#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <SDL/SDL.h>

namespace ECSL
{
	class DECLSPEC Subscription
	{
	public:
		Subscription(unsigned int _subscriberId, unsigned int _senderId, unsigned int _messageType, void* _onEventFunction);
		~Subscription();

		unsigned int GetSubscriberId() { return m_subscriberId; }
		unsigned int GetSenderId() { return m_senderId; }
		unsigned int GetMessageType() { return m_messageType; }
		void* GetOnEventFunction() { return m_onEventFunction; }

	private:
		unsigned int m_subscriberId;
		unsigned int m_senderId;
		unsigned int m_messageType;
		void* m_onEventFunction;
	};
}

#endif