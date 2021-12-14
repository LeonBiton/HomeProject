#include "event_router.hpp"

EventRouter::EventRouter(FindSubscriber &a_finder, size_t a_queueSize , int a_numOfThreads)
:m_notifyer(a_finder , a_queueSize , a_numOfThreads){}


void EventRouter::SendEvent(std::shared_ptr<IEvent> a_event)
{
    m_notifyer.GenreateTasks(a_event);
}
