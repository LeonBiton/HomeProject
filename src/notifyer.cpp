#include "notifyer.hpp"

Notifyer::Notifyer(FindSubscriber &a_finder , size_t a_queueSize , int a_numOfThreads)
:m_finder(a_finder)
,m_threadPool(a_queueSize , a_numOfThreads){}


void Notifyer::GenreateTasks(std::shared_ptr<IEvent> a_event)
{
    agentVector subscribers = m_finder.FindSubscribers(a_event->GetEventTopic());
    agentVector::iterator vectorItr = subscribers.begin();
    while(vectorItr != subscribers.end())
    {
        Task * newTask = new Task(*vectorItr , a_event);
        std::shared_ptr<Callable> task(newTask);
        m_threadPool.Submit(task);
        ++vectorItr;
    }
}

