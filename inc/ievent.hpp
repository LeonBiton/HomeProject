#ifndef IEVENT_HPP
#define IEVENT_HPP
#include <string>
#include <ctime>
#include "topic.hpp"

namespace advcpp
{

class IEvent{
public:
    IEvent(EventTopic a_eventTopic);
    ~IEvent() = default;

    std::string GetEventType() const;
    EventTopic GetEventTopic() const;
    std::time_t GetTimestamp() const;

private:
    EventTopic m_eventTopic;
    std::time_t m_timestamp;
};
}


#endif