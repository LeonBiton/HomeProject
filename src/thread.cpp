#include "thread.hpp"

using namespace advcpp;

SyncPoint::SyncPoint()
:m_flag(false){}


void SyncPoint::Set(){
    m_flag.Set();
}

bool SyncPoint::IsSet(){
    return m_flag.IsSet();
}

Thread::Thread(std::shared_ptr<Callable> a_callable , POLICY a_policy)
:m_thread(),
m_task(a_callable),
m_sem(0,0),
m_syncP(new SyncPoint),
m_isJoined(false),
m_isDetached(false),
m_isCanceled(false),
m_policy(a_policy){
    int error = pthread_create(&m_thread, 0, ToThread, this);
    if(error){
        std::runtime_error("thread create fail");
    }
    m_sem.Wait();
}

Thread::~Thread(){
    if(m_isDetached.IsSet() == false && m_isJoined.IsSet() == false && m_isCanceled.IsSet() == false){
        switch(m_policy){
            case JOIN:
                Join();
            case DETACH:
                Detach();
            case ASSERT:
                assert("destroy thread without join/detach/cancell");
        }
    }
}

void* Thread::ToThread(void *a_arg){
    assert(a_arg != 0);
    Thread *self = static_cast<Thread *>(a_arg);
    std::shared_ptr<Callable> call = self->m_task;
    std::shared_ptr<SyncPoint> syncPointer = self->m_syncP;
    self->m_sem.Post();
    try{
        call->Run();
    }
    catch(std::exception const & exception){
        assert("thread failed");
    }
    syncPointer->Set();
    return nullptr;
}


void Thread::Detach(){
    if(m_isDetached.IsSet() == false && m_isJoined.IsSet() == false){
        if(m_startedDetaching.TrySet()){
            int error = pthread_detach(m_thread);
            if(error){
                throw std::runtime_error("detach failed");
            }
            m_isDetached.Set();
        }

    }
}

void Thread::Join(){
    if(m_isDetached.IsSet() == false && m_isJoined.IsSet() == false){
        if(m_startedJoining.TrySet()){
            int error = pthread_join(m_thread,NULL);
            if(error){
                throw std::runtime_error("join failed");
            }
            m_isJoined.Set();
        }
    }
}

void Thread::Cancel(const bool &a_isJoin){
    int error = pthread_cancel(m_thread);
    if(error){
        throw std::runtime_error("cancel failed");
    }
    if(a_isJoin){
        Join();
    }
    m_isCanceled.Set();
}

bool Thread::isThreadDone()const{
    return m_syncP->IsSet();
}
