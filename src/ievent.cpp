#include "ievent.hpp"

using namespace advcpp;

IEvent::IEvent(EventTopic a_EventTopic)
:m_eventTopic(a_EventTopic)
,m_timestamp(std::time(0)){}


std::string IEvent::GetEventType() const
{
    return m_eventTopic.GetType();
}

EventTopic IEvent::GetEventTopic() const
{
    return m_eventTopic;
}

std::time_t IEvent::GetTimestamp() const
{
    return m_timestamp;
}
