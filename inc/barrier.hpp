#ifndef BARRIER_HPP
#define BARRIER_HPP
#include <stdexcept>
#include <assert.h>
#include <pthread.h>

namespace advcpp{

class Barrier{
public:
    Barrier(size_t a_number);
    ~Barrier();

    Barrier(const Barrier& a_other) = delete;
    Barrier& operator=(const Barrier& a_other) = delete;

    void Wait();

private:
    pthread_barrier_t m_barrier;

};


}


#endif
