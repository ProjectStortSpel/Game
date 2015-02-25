#include "Subscription.h"

using namespace ECSL;

Subscription::Subscription(const std::string& _subscriberName, const std::string& _subscribesToName, unsigned int _messageType)
: m_subscriberName(_subscriberName), m_subscribesToName(_subscribesToName), m_messageType(_messageType)
{

}

Subscription::~Subscription()
{

}