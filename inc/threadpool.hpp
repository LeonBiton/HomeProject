#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <cxxabi.h>
#include "blocking_bounded_queue.hpp"
#include "thread_group.hpp"
#include "funcadapter.hpp"
#include "barrier.hpp"

using namespace advcpp;

namespace advcpp{


class PoisonApple : public Callable{
public:
    PoisonApple(){};

private:
    void Run() override{};

};


class ThreadPool{
public:

    ThreadPool(size_t a_queueSize , int a_numOfThreads);
    ~ThreadPool();

    void Submit(std::shared_ptr<Callable> a_task);
    void Shutdown();
    void ShutdownImmediate();

    void Add(size_t a_numberOfThreads);
    void Remove(size_t a_numberOfThreads);

    size_t TasksInProgress() const;
    size_t NumberOfThreads() const;

private:
    class RunThread : public Callable{
    public:
        RunThread(ThreadPool & a_threadpool);

    private:
        void Run() override;

    private:
        ThreadPool *m_threadpool;

    };

    class PoisonApple : public Callable{
    public:
        PoisonApple(){};

    private:
        void Run() override{};
    };

private:
    BlockingBoundedQueue<std::shared_ptr<Callable>> m_tasks;
    std::shared_ptr<Callable> m_exitTask;
    ThreadGroup m_workers;
    AtomicValue<bool> m_shutdown;
    AtomicValue<size_t> m_removeCounter;
};

}


#endif