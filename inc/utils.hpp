#ifndef UTILS_HPP
#define UTILS_HPP
#include "topic.hpp"

using namespace advcpp;


struct HashFunc
{
    size_t operator()(const EventTopic & a_eventTopic) const;
};

#endif