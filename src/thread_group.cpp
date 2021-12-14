#include "thread_group.hpp"


struct Joiner{
    void operator()(std::shared_ptr<Thread> a_thread){
        try{
        a_thread->Join();}
        catch(std::exception const &except){
            std::cerr << except.what() << std::endl;    
        }
    }

};

struct Detacher{
    void operator()(std::shared_ptr<Thread> a_thread){
        try{ 
        a_thread->Detach();}
        catch(std::exception const &except){
            std::cerr << except.what() << std::endl;     
        }
    }
};

struct Canceler{
    Canceler(bool const &a_isJoin)
    :m_isJoined(a_isJoin){}

    void operator()(std::shared_ptr<Thread> a_thread){
        try{ 
        a_thread->Cancel(m_isJoined);}
        catch(std::exception const &except){
            std::cerr << except.what() << std::endl;     
        }
    }

    bool m_isJoined;
};

ThreadGroup::ThreadGroup(int a_size , std::shared_ptr<Callable> a_callable)
:m_workers(),
m_isAllDetached(false),
m_isAllJoined(false),
m_isAllCanceled(false),
m_callable(a_callable),
m_mutex(){
    for(int i = 0; i < a_size ; ++i){
        Thread * t = new Thread(a_callable , Thread::JOIN);
        m_workers.push_back(std::shared_ptr<Thread>(t));
    }
}

void ThreadGroup::JoinAll(){
    std::for_each(m_workers.begin(), m_workers.end() , Joiner());
    m_isAllJoined = true;
}

void ThreadGroup::DetachAll(){
    std::for_each(m_workers.begin(), m_workers.end() , Detacher());
    m_isAllDetached = true;
}

void ThreadGroup::CancelAll(bool const &a_isJoin){
    std::for_each(m_workers.begin(), m_workers.end() , Canceler(a_isJoin));
}

void ThreadGroup::Add(size_t const &a_numberOfThreads){
    Guard g(m_mutex);
    RemoveDoneThreads();
    for(size_t i = 0; i < a_numberOfThreads ; ++i){
        m_workers.emplace_back(new Thread(m_callable , Thread::JOIN));
    }
}

void ThreadGroup::Remove(size_t const &a_numberOfThreads){
    assert(m_workers.size() >= a_numberOfThreads);
    Guard g(m_mutex);
    size_t removed = RemoveDoneThreads();
    if(removed < a_numberOfThreads){
        for(size_t i = 0 ; i < (a_numberOfThreads - removed) ; ++i){
            m_workers.back()->Cancel();
            m_workers.pop_back();
        }
    }
}

size_t ThreadGroup::RemoveDoneThreads(){
    size_t removed = 0;
    std::vector<std::shared_ptr<Thread>>::iterator it = m_workers.begin();
    while(it != m_workers.end()){
        if((*it)->isThreadDone()){
            it = m_workers.erase(it);
            ++removed;
        }
        else{
            ++it; 
        }
    }
    return removed;
}

size_t ThreadGroup::GetNumberOfThreads() const
{
    return m_workers.size();
}