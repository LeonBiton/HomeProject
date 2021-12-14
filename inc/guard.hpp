#ifndef GUARD_HPP
#define GUARD_HPP
#include <pthread.h>
#include <mutex>

class Guard{
public:
    Guard(std::mutex& a_mutex);
    ~Guard();
private:
    std::mutex &m_mutex;
};

#endif