#ifndef THREAD_HPP
#define THREAD_HPP
#include <iostream>
#include <pthread.h> //for threads
#include <stdexcept>
#include <memory>
#include <assert.h>
#include <utility>
#include <pthread.h>
#include "callable.hpp"
#include "semaphore.hpp"
#include "inl/atomic.hxx"

namespace advcpp{


class SyncPoint{
public:
    SyncPoint();

    void Set();
    bool IsSet();

private:
    AtomicValue<bool> m_flag;
};

class Thread{
public:
    enum POLICY{JOIN , DETACH , ASSERT};
    Thread(std::shared_ptr<Callable> a_callable , POLICY a_policy);

    ~Thread();

    Thread& operator=(const Thread &a_thread) = delete;
    Thread(const Thread &a_thread) = delete;

    void Detach();
    void Join();
    void Cancel(const bool &a_isJoin = true);

    bool isThreadDone() const;

private:
    static void* ToThread(void *a_arg);

private:
    pthread_t m_thread;
    std::shared_ptr<Callable> m_task;
    Semaphore m_sem;
    std::shared_ptr<SyncPoint> m_syncP;

    AtomicValue<bool> m_isJoined;
    AtomicValue<bool> m_isDetached;
    AtomicValue<bool> m_isCanceled;
    AtomicValue<bool> m_startedDetaching;
    AtomicValue<bool> m_startedJoining;

    POLICY m_policy;
};
}

#endif
