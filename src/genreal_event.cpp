#include "genreal_event.hpp"

using namespace advcpp;

GeneralEvent::GeneralEvent(EventTopic a_topic , std::string a_data)
:IEvent(a_topic)
, m_data(a_data){}