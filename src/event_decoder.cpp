#include "event_decoder.hpp"


IEvent* EventDecoder::Decode(std::string a_buffer)
{
    json j = json::parse(a_buffer);
    GeneralEvent * event = new GeneralEvent (EventTopic(j["Type"] , j["Floor"].get<int>() ,j["Room"].get<int>()) , j["Data"]);
    return event;
}
