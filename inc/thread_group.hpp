#ifndef THREAD_GROUP_HPP
#define THREAD_GROUP_HPP
#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include "thread.hpp"
#include "guard.hpp"

using namespace advcpp;

namespace advcpp{
class ThreadGroup{
public:
    ThreadGroup(int a_size , std::shared_ptr<Callable> a_callable);
    ~ThreadGroup() = default;

    void JoinAll();
    void DetachAll();
    void CancelAll(bool const &a_isJoin = true);

    void Add(size_t const &a_numberOfThreads);
    void Remove(size_t const &a_numberOfThreads);

    size_t GetNumberOfThreads()const;
    
private:
    size_t RemoveDoneThreads();

private:
    std::vector<std::shared_ptr<Thread>> m_workers;

    bool m_isAllDetached;
    bool m_isAllJoined;
    bool m_isAllCanceled;
    std::shared_ptr<Callable> m_callable;
    std::mutex m_mutex;
};
}

#endif