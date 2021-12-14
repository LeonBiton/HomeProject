#ifndef EVENT_TASK_HPP
#define EVENT_TASK_HPP
#include <memory>
#include "callable.hpp"
#include "iagent.hpp"

using namespace advcpp;

namespace advcpp
{

class Task : public Callable
{
public:
    Task(std::shared_ptr<IAgent> a_agent ,std::shared_ptr<IEvent> a_event);

    void Run() override;

private:
    std::shared_ptr<IAgent> m_agent;
    std::shared_ptr<IEvent> m_event;

};
}

#endif