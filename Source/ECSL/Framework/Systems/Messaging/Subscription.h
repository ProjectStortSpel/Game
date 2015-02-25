#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <SDL/SDL.h>
#include <string>

namespace ECSL
{
	class Subscription
	{
	public:
		Subscription(const std::string& _subscriberName, const std::string& _subscribesToName, unsigned int _messageType);
		~Subscription();

		unsigned int GetSubscriberId() { return m_subscriberId; }
		unsigned int GetSubscribesToId() { return m_subscribesToId; }
		unsigned int GetMessageType() { return m_messageType; }
		const std::string& GetSubscriberName() { return m_subscriberName; }
		const std::string& GetSubscribesToName() { return m_subscribesToName; }

		void SetSubscriberId(unsigned int _id) { m_subscriberId = _id; }
		void SetSubscribesToId(unsigned int _id) { m_subscribesToId = _id; }

	private:
		unsigned int m_subscriberId;
		unsigned int m_subscribesToId;
		unsigned int m_messageType;
		std::string m_subscriberName;
		std::string m_subscribesToName;
	};
}

#endif