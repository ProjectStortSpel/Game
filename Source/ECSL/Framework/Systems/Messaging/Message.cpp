#include "Message.h"

using namespace ECSL;

Message::Message()
{

}

Message::~Message()
{
	if (m_senderName)
		delete(m_senderName);
}

void Message::Initialize(unsigned int _messageType, const std::string& _senderName, unsigned int _senderId)
{
	m_messageType = _messageType;
	m_senderName = new std::string(_senderName);
	m_senderId = _senderId;
}