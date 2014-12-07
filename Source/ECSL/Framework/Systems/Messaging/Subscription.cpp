#include "Subscription.h"

using namespace ECSL;

Subscription::Subscription(unsigned int _subscriberId, unsigned int _senderId, unsigned int _messageType, void* _onEventFunction)
: m_subscriberId(_subscriberId), m_senderId(_senderId), m_messageType(_messageType), m_onEventFunction(_onEventFunction)
{

}

Subscription::~Subscription()
{

}