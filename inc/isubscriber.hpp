#ifndef ISUBSCRIBER_HPP
#define ISUBSCRIBER_HPP
#include <string>
#include "topic.hpp"
#include "iagent.hpp"


namespace advcpp
{

class ISubscribers{
public:
    ISubscribers() = default;
    ~ISubscribers() = default;

    virtual void Subscribe(EventTopic a_eventTopic, std::shared_ptr<IAgent> a_agent) = 0;
    virtual void Unsubscribe(EventTopic a_eventTopic, std::shared_ptr<IAgent> a_agent) = 0;

    virtual bool IsDeviceSubscribe(EventTopic a_eventLocation, std::shared_ptr<IAgent> a_agent) const = 0; 
    virtual size_t GetSize() const = 0;
};
}

#endif
