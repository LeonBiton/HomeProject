#ifndef BLOCKING_BOUNDED_QUEUE_HXX
#define BLOCKING_BOUNDED_QUEUE_HXX
#include "../blocking_bounded_queue.hpp"

using namespace advcpp;

template <typename T>
BlockingBoundedQueue<T>::BlockingBoundedQueue(size_t a_capacity)
:m_queue(),
m_mutex(),
m_capacity(a_capacity),
m_waitersFull(0),
m_waitersEmpty(0),
m_isAlive(true){}

template <typename T>
BlockingBoundedQueue<T>::~BlockingBoundedQueue(){
    m_isAlive.Clear();
    m_empty.notify_all();
    m_full.notify_all();
    while(m_waitersEmpty.GetValue() || m_waitersFull.GetValue());//TODO: change to inturupt
}


template <typename T>
void BlockingBoundedQueue<T>::Enqueue(const T &a_item , bool IsPrioritize){
    if(!m_isAlive.IsSet()){
        return;
    }
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_queue.size() == m_capacity.GetValue()) {
        ++m_waitersFull;
        m_full.wait(lock);
        --m_waitersFull;
    }
    if(!m_isAlive.IsSet()){
        return;
    }
    try{
        if(IsPrioritize)
        {
            m_queue.push_front(a_item);
        }
        else
        {
            m_queue.push_back(a_item);
        }
    }catch(...){
        lock.unlock();
        if(m_waitersEmpty.GetValue()){
        m_empty.notify_one(); 
        } 
        throw;
    }
    lock.unlock();
    if(m_waitersEmpty.GetValue()){
        m_empty.notify_one(); 
    }
}

template <typename T>
bool BlockingBoundedQueue<T>::Dequeue(T &a_returnValue){
    if(!m_isAlive.IsSet()){
        return false;
    }
    std::unique_lock<std::mutex> lock(m_mutex);
    while(m_queue.empty()){
        ++m_waitersEmpty;
        m_empty.wait(lock);
        --m_waitersEmpty;
    }
    if(!m_isAlive.IsSet()){
        return false;
    }
    try{
        a_returnValue = m_queue.front();
        m_queue.pop_front();
    }catch(...){
        lock.unlock();
        if(m_waitersFull.GetValue()){
        m_full.notify_one();
        }
        throw;
    }
    lock.unlock();
    if(m_waitersFull.GetValue()){
        m_full.notify_one();
    }
    return true;
}

template <typename T>
bool BlockingBoundedQueue<T>::IsEmpty() const{
    Guard g(m_mutex);
    return m_queue.empty();
}

template <typename T>
bool BlockingBoundedQueue<T>::IsFull() const{
    Guard g(m_mutex);
    return (m_queue.size() == m_capacity);
}

template <typename T>
size_t BlockingBoundedQueue<T>::Size() const{
    Guard g(m_mutex);
    return m_queue.size();
}

template <typename T>
size_t BlockingBoundedQueue<T>::Capacity() const{
    Guard g(m_mutex);
    return m_capacity;
}

template <typename T>
bool BlockingBoundedQueue<T>::IsValid() const
{
    return m_isAlive.IsSet();
}

#endif
