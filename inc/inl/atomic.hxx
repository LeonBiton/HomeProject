#include "atomic.hpp"

using namespace advcpp;

template<typename T>
AtomicValue<T>::AtomicValue(T a_value)
{
    SetValue(a_value);
}


template<typename T>
void AtomicValue<T>::SetValue(T a_newValue){
    T oldValue;
    T actualOldValue;
    do {
        oldValue = GetValue();
        actualOldValue = __sync_val_compare_and_swap(&m_value, oldValue, a_newValue);
    } while(oldValue != actualOldValue);
}

template<typename T>
inline T AtomicValue<T>::GetValue() const{
   return __sync_fetch_and_add(&m_value , T());
}

template<typename T>
T AtomicValue<T>::operator++(int){
    return __sync_fetch_and_add(&m_value, 1);
}

template<typename T>
T AtomicValue<T>::operator++(){
    return operator+=(1);
}

template<typename T>
T AtomicValue<T>::operator--(int){
    return __sync_fetch_and_sub(&m_value , 1);
}

template<typename T>
T AtomicValue<T>::operator--(){
    return operator-=(1);
}

template<typename T>
T AtomicValue<T>::operator-=(T a_delta){
    return __sync_sub_and_fetch(&m_value , a_delta);
}

template<typename T>
T AtomicValue<T>::operator+=(T a_delta){
    return __sync_add_and_fetch(&m_value, a_delta);
}

inline AtomicValue<bool>::AtomicValue(bool a_flag)
: m_flag(a_flag)
{}

inline void AtomicValue<bool>::Set(){
    __sync_or_and_fetch(&m_flag , 1);
}

inline bool AtomicValue<bool>::TrySet(){
   return __sync_bool_compare_and_swap(&m_flag , 0 , 1);
}

inline void AtomicValue<bool>::Clear(){
    __sync_and_and_fetch(&m_flag , 0);
}

inline bool AtomicValue<bool>::IsSet()const{
    return __sync_and_and_fetch(&m_flag , 1);
}
