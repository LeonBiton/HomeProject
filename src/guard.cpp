#include "guard.hpp"

Guard::Guard(std::mutex& a_mutex)
:m_mutex(a_mutex){m_mutex.lock();}

Guard::~Guard(){
m_mutex.unlock();}