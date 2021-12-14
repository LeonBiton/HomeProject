#ifndef FUNCADAPTER_HPP
#define FUNCADAPTER_HPP
#include <iostream>
#include <vector>
#include <mutex>
#include "thread.hpp"
#include "blocking_bounded_queue.hpp"


namespace advcpp{
template <typename Function>
class ActionFunction : public Callable {
public:
    ActionFunction(Function (&a_function) ());

private:
    void Run() override;

private:
    Function (&m_function) ();
};

template <typename ResT>
class GlobalFunction : public Callable {
public:
    GlobalFunction(ResT (&a_function) ());

    ResT GetValue()const;

private:
    void Run() override;

private:
    ResT (&m_function) ();
    ResT m_value;
};

template <typename ResT , typename ParamT>
class GlobalFunction1 : public Callable {
public:
    GlobalFunction1(ResT (&a_function) (ParamT) , ParamT a_param);

    ResT GetValue()const;

private:
    void Run() override;

private:
    ResT (&m_function) (ParamT);
    ParamT m_param;
    ResT m_value;
};

template <typename Class>
class MemberActionFunction : public Callable{
public:
    MemberActionFunction(void(Class::* a_function)() , Class & a_this);

private:
    void Run() override;

private:
    void(Class::* m_function)();
    Class * m_this;

};

template <typename ResT , typename Class , typename  Param>
class MemberFunction1 : public Callable{
public:
    MemberFunction1(ResT(Class::* a_function)(Param), Class & a_this , Param a_param);

    ResT GetValue()const{return m_value;}

private:
    void Run() override;

private:
    ResT(Class::* m_function)(Param);
    Class * m_this;
    Param m_param;
    ResT m_value;
};

template <typename ResT , typename Class , typename  Param1 ,typename Param2>
class MemberFunction2 : public Callable{
public:
    MemberFunction2(ResT(Class::* a_function)(Param1 , Param2), Class & a_this ,
Param1 a_param , Param2 a_param2);

    ResT GetValue()const{return m_value;}

private:
    void Run() override;

private:
    ResT(Class::* m_function)(Param1 , Param2);
    Class * m_this;
    Param1 m_param;
    Param2 m_param2;
    ResT m_value;

};

template <typename T>
class Consumer : public Callable{
public:
    Consumer(BlockingBoundedQueue<T> &a_queue , size_t a_size);

    std::vector<T>const & GetVector()const;

private:
    void Run() override;

private:
    BlockingBoundedQueue<T> &m_queue;
    std::vector<T> m_vector;
    size_t m_size;

};

template <typename T>
class Producer : public Callable{
public:
    Producer(BlockingBoundedQueue<T> &a_queue , size_t a_size , T a_item);

    std::vector<T>const & GetVector()const;

private:
    void Run() override;

private:
    BlockingBoundedQueue<T> &m_queue;
    std::vector<T> m_vector;
    size_t m_size;
    T m_item;
};

template <typename T>
class ProducerWithVec : public Callable{
public:
    ProducerWithVec(BlockingBoundedQueue<T> &a_queue, std::vector<T> &a_pool
 , std::mutex &a_mutex , size_t a_size);

    std::vector<T>const & GetVector()const;

private:
    void Run() override;

private:
    BlockingBoundedQueue<T> &m_queue;
    std::mutex &m_mutex;
    std::vector<T> &m_pool;
    std::vector<T> m_vector;
    size_t m_size;
};

template<typename T>
class NewProducer : public Callable{
public:
    NewProducer(BlockingBoundedQueue<T> &a_queue , size_t from , size_t to);

private:
    void Run() override;

private:
    BlockingBoundedQueue<T> &m_queue;
    size_t m_from;
    size_t m_to;

};

template<typename T>
class NewConsumer : public Callable{
public:
    NewConsumer(BlockingBoundedQueue<T>& a_queue
,std::vector<std::pair<size_t, size_t>> const& a_categories, size_t a_size);

    bool IsOk()const;

private:
    void Run() override;
    size_t FindRange(T a_item);

private:
    BlockingBoundedQueue<T>& m_queue;
    std::vector<std::pair<size_t, size_t>> m_categories;
    size_t m_expected;
    bool m_isOk;
};

#include "inl/funcadapter.hxx"
}

#endif
