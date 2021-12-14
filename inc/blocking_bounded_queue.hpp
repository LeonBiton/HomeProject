#ifndef BLOCKING_BOUNDED_QUEUE_HPP
#define BLOCKING_BOUNDED_QUEUE_HPP
#include <queue>
#include <deque>
#include <mutex>
#include <condition_variable>
#include "atomic.hpp"
#include "guard.hpp"

namespace advcpp{


template <typename T>
class BlockingBoundedQueue{
public:
    explicit BlockingBoundedQueue(size_t a_capacity);
    ~BlockingBoundedQueue();

    BlockingBoundedQueue& operator=(const BlockingBoundedQueue &a_queue) = delete;
    BlockingBoundedQueue(const BlockingBoundedQueue &a_queue) = delete;

    void Enqueue(const T &a_item , bool IsPrioritize = false);
    bool Dequeue(T &a_returnValue);

    bool IsEmpty() const;
    bool IsFull() const;
    size_t Size() const;
    size_t Capacity() const;

    bool IsValid() const;

private:
    std::deque<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_empty;
    std::condition_variable m_full;

    AtomicValue<size_t> m_capacity;
    AtomicValue<int> m_waitersFull;
    AtomicValue<int> m_waitersEmpty;
    AtomicValue<bool> m_isAlive;

};
#include "inl/blocking_bounded_queue.hxx"
}

#endif