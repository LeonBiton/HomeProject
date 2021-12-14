#include "barrier.hpp"

using namespace advcpp;

Barrier::Barrier(size_t a_number)
:m_barrier()
{
    if (pthread_barrier_init(&m_barrier, 0, a_number))
    {
        throw std::runtime_error("barrier create failed");
    }
}


Barrier::~Barrier()
{
    int res = pthread_barrier_destroy(&m_barrier);
    assert(res == 0);
}

void Barrier::Wait()
{
    if (pthread_barrier_wait(&m_barrier))
    {
        throw std::runtime_error("barrier wait failed");
    }
}