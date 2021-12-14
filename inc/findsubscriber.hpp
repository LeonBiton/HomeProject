#ifndef FINDSUBSCRIBER_HPP
#define FINDSUBSCRIBER_HPP
#include <vector>
#include "isubscriber.hpp"
#include "ievent.hpp"
#include "topic.hpp"

using namespace advcpp;


namespace advcpp
{

class FindSubscriber{
public:
    virtual ~FindSubscriber() = default;

    virtual std::vector<std::shared_ptr<IAgent>> FindSubscribers(EventTopic a_eventTopic) = 0;

private:

};
}


#endif