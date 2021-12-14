#include "utils.hpp"

size_t HashFunc::operator()(const EventTopic & a_eventTopic) const
{
    return a_eventTopic.HashFunc();
}