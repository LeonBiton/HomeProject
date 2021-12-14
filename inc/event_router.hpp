#ifndef EVENT_ROUTER_HPP
#define EVENT_ROUTER_HPP
#include "ievent.hpp"
#include "notifyer.hpp"

using namespace advcpp;


namespace advcpp
{
class EventRouter
{
public:
    EventRouter(FindSubscriber &a_finder, size_t a_queueSize , int a_numOfThreads);

    void SendEvent(std::shared_ptr<IEvent> a_event);

private:
    Notifyer m_notifyer;

};


}


#endif