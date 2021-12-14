#ifndef IAGENT_HPP
#define IAGENT_HPP
#include <memory>
#include "topic.hpp"
#include "ievent.hpp"
#include "tcpclient.hpp"

using namespace advcpp;

namespace advcpp
{

class IAgent{
public:
    IAgent(size_t a_id);
    ~IAgent() = default;

    virtual void Notify(std::shared_ptr<IEvent> a_event) = 0;
    virtual void Subscribe(EventTopic a_eventTopic) = 0;
    virtual void Unsubscribe(EventTopic a_eventTopic) = 0;

    size_t GetId() const;

private:
    size_t m_id;
};

}

#endif