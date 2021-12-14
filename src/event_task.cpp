#include "event_task.hpp"

Task::Task(std::shared_ptr<IAgent> a_agent ,std::shared_ptr<IEvent> a_event)
:m_agent(a_agent)
,m_event(a_event){}

void Task::Run()
{
    m_agent->Notify(m_event);
}
