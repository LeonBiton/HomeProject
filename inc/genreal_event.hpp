#ifndef GENERAL_EVENT_HPP
#define GENERAL_EVENT_HPP
#include "ievent.hpp"

using namespace advcpp;

namespace advcpp{

class GeneralEvent : public IEvent
{
public:
    GeneralEvent(EventTopic a_topic , std::string a_data);

private:
std::string m_data;


};



}


#endif