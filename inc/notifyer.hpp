#ifndef NOTIFYER_HPP
#define NOTIFYER_HPP
#include <memory>
#include "findsubscriber.hpp"
#include "callable.hpp"
#include "event_task.hpp"
#include "threadpool.hpp"


using namespace advcpp;

namespace advcpp
{

typedef std::vector<std::shared_ptr<IAgent>> agentVector;

class Notifyer{
public:
    Notifyer(FindSubscriber &a_finder , size_t a_queueSize , int a_numOfThreads);

    void GenreateTasks(std::shared_ptr<IEvent> a_event);

private:
    FindSubscriber &m_finder;
    ThreadPool m_threadPool;
};


}


#endif