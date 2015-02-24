#ifndef MESSAGE_H
#define MESSAGE_H

#include <SDL/SDL.h>
#include <string>

namespace ECSL
{
	class DECLSPEC Message
	{
	public:
		Message();
		virtual ~Message() = 0;

		void Initialize(unsigned int _messageType, const std::string& _senderName, unsigned int _senderId);

		unsigned int GetMessageType() { return m_messageType; }
		const std::string& GetSenderName() { return *m_senderName; }
		unsigned int GetSenderId() { return m_senderId; }

	private:
		unsigned int m_messageType;
		std::string* m_senderName;
		unsigned int m_senderId;
	};

	class TestMessage : public Message
	{
	public:
		TestMessage() { }
	private:
	};
}

#endif