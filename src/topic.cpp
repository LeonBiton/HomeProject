#include "topic.hpp"

using namespace advcpp;

EventTopic::EventTopic(std::string a_type , int a_floor , int a_room, bool a_allFloor)
:m_type(a_type)
,m_floor(a_floor)
,m_room(a_room)
,m_allFloor(a_allFloor){}

EventTopic::EventTopic()
:m_type()
,m_floor(0)
,m_room(0)
,m_allFloor(false){}

/*EventTopic::EventTopic(std::string a_type , std::string a_floor , std::string a_room , int a_dummy , bool a_allFloor)
:m_type(a_type)
,m_floor(atoi(a_floor.c_str()))
,m_room(atoi(a_room.c_str()))
,m_allFloor(a_allFloor){}*/


bool EventTopic::operator==(const EventTopic &a_otherEventTopic) const
{
    if(m_type == a_otherEventTopic.m_type && m_room == a_otherEventTopic.m_room &&
    m_floor == a_otherEventTopic.m_floor)
    {
        return true;
    }
    return false;
}

std::string EventTopic::GetType() const
{
    return m_type;
}

size_t EventTopic::HashFunc() const
{
    std::string res = std::to_string(m_floor + m_room);
    return std::hash<std::string>()(res+m_type);
}

int EventTopic::GetFloor() const
{
    return m_floor;
}

int EventTopic::GetRoom() const
{
    return m_room;
}
