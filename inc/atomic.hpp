#ifndef ATOMIC_HPP
#define ATOMIC_HPP

#include <iostream>

namespace advcpp{
template<typename T>
class AtomicValue{
public:
    AtomicValue(T a_value);

    AtomicValue(const AtomicValue &a_atomicValue) = delete;
    AtomicValue& operator=(const AtomicValue &a_atomicValue) = delete;

    void SetValue(const T a_value);
    T GetValue()const;

    T operator++();
    T operator++(int);
    T operator--();
    T operator--(int);
    T operator+=(T a_delta);
    T operator-=(T a_delta);

private:
    mutable T m_value;
};

template<>
class AtomicValue<bool>{
public:
    AtomicValue(bool a_flag = false);

    AtomicValue(const AtomicValue &a_atomicValue) = delete;
    AtomicValue& operator=(const AtomicValue &a_atomicValue) = delete;

    void Set();
    bool TrySet();
    void Clear();
    bool IsSet() const;

private:
    mutable unsigned char m_flag;
};
}
//TODO: include inl file

#endif
