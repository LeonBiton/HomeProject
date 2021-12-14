#ifndef TOPIC_HPP
#define TOPIC_HPP
#include <string>

namespace advcpp
{

class EventTopic{
public:
    EventTopic(std::string a_type , int a_floor , int a_room , bool a_allFloor = false);
    EventTopic();

    bool operator==(const EventTopic &a_otherEventTopic) const;

    std::string GetType() const;
    int GetFloor() const;
    int GetRoom() const;

    size_t HashFunc() const;

private:
    std::string m_type;
    int m_floor;
    int m_room;
    bool m_allFloor;
};
}
#endif