#include "semaphore.hpp"

using namespace advcpp;


Semaphore::Semaphore(int a_pshared , unsigned int a_semVal){
    sem_init(&m_semaphore , a_pshared , a_semVal);
}

Semaphore::~Semaphore(){
    sem_destroy(&m_semaphore);
}

int Semaphore::Wait(){
    return sem_wait(&m_semaphore);
}

int Semaphore::Post(){
    return sem_post(&m_semaphore);
}

int Semaphore::GetValue(int * a_sval){
    return sem_getvalue(&m_semaphore , a_sval);
}