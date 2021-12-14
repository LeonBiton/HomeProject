#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <iostream>
#include <mutex>

class Counter{
public:
    Counter(size_t a_count);

    void Increment();
    size_t IncrementLoop(size_t n);

    void Decrement();
    size_t DecrementLoop(size_t n);

    size_t GetCount()const{return m_count;};

private:
    size_t m_count;
    std::mutex m_mutex;
};

#endif