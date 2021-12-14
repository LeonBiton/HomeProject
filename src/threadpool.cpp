#include "threadpool.hpp"
#include <unistd.h>


ThreadPool::RunThread::RunThread(ThreadPool & a_threadpool)
:m_threadpool(&a_threadpool){}

void ThreadPool::RunThread::Run()
{
    while(true)
    {
        std::shared_ptr<Callable> task;
        try
        {
            m_threadpool->m_tasks.Dequeue(task);
        }catch(...)
        {
            throw;
        }
        if(task)
        {
            if(task == m_threadpool->m_exitTask)
            {
                ++m_threadpool->m_removeCounter;
                break;
            }
            try
            {
                task->Run();
            }
            catch(const abi::__forced_unwind &e)
            {
                throw;
            }
            catch(...)
            {   
            }
        }
    }
}

ThreadPool::ThreadPool(size_t a_queueSize , int a_numOfThreads) //TODO: dufault threads is func that gets num of threads
:m_tasks(a_queueSize),
m_exitTask(std::shared_ptr<Callable>(new PoisonApple)),
m_workers(a_numOfThreads , std::shared_ptr<Callable>(new RunThread(*this))),
m_shutdown(false),
m_removeCounter(0)
{
}

ThreadPool::~ThreadPool()
{
    Shutdown();
}

void ThreadPool::Submit(std::shared_ptr<Callable> a_task)
{
    if(m_shutdown.IsSet())
    {
        return;
    }
    m_tasks.Enqueue(a_task);
}

void ThreadPool::Shutdown()
{
    if(m_shutdown.IsSet())
    {
        return;
    }
    m_shutdown.Set();
    for(size_t i = 0 ; i < m_workers.GetNumberOfThreads() ; ++i)
    {
        m_tasks.Enqueue(m_exitTask);
    }
    m_workers.JoinAll();
}

void ThreadPool::ShutdownImmediate()
{
    if(m_shutdown.IsSet())
    {
        return;
    }
    m_shutdown.Set();
    for(size_t i = 0 ; i < m_workers.GetNumberOfThreads() ; ++i)
    {
        m_tasks.Enqueue(m_exitTask , true);
    }
}

void ThreadPool::Add(size_t a_numberOfThreads)
{
    m_workers.Add(a_numberOfThreads);
}

void ThreadPool::Remove(size_t a_numberOfThreads)
{
    for(size_t i = 0 ; i < a_numberOfThreads ; ++i)
    {
        m_tasks.Enqueue(m_exitTask);
    }
    while(m_removeCounter.GetValue() < a_numberOfThreads);//TODO: busy wait
    m_workers.Remove(a_numberOfThreads);
}

size_t ThreadPool::TasksInProgress()const
{
    return m_tasks.Size();
}

size_t ThreadPool::NumberOfThreads()const
{
    return m_workers.GetNumberOfThreads();
}

