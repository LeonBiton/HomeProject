#ifndef SUBSCRIBERSMANAGER_HPP
#define SUBSCRIBERSMANAGER_HPP

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <exception>
#include "isubscriber.hpp"
#include "topic.hpp"
#include "utils.hpp"
#include "findsubscriber.hpp"

using namespace advcpp;

namespace advcpp
{
typedef std::vector<std::shared_ptr<IAgent>> agentVector;

class SubscribersManager : public ISubscribers , public FindSubscriber{
public:
    void Subscribe(EventTopic a_eventTopic, std::shared_ptr<IAgent> a_agent) override;
    void Unsubscribe(EventTopic a_eventTopic, std::shared_ptr<IAgent> a_agent) override;

    agentVector FindSubscribers(EventTopic a_eventTopic) override;

    bool IsDeviceSubscribe(EventTopic a_eventLocation, std::shared_ptr<IAgent> a_agent) const;

    size_t HashSize() const;
    size_t GetSize() const override;
private:
    std::unordered_map<EventTopic,agentVector,HashFunc> m_hash;

};
}
#endif