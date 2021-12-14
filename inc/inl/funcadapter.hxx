#ifndef FUNCADAPTER_HXX
#define FUNCADAPTER_HXX
#include "funcadapter.hpp"


template <typename Function>
ActionFunction<Function>::ActionFunction(Function (&a_function)())
:m_function(a_function){}

template <typename Function>
void ActionFunction<Function>::Run(){
    m_function();
}

template <typename ResT>
GlobalFunction<ResT>::GlobalFunction(ResT (&a_function) ())
:m_function(a_function){}

template <typename ResT>
void GlobalFunction<ResT>::Run(){
   m_value = m_function();
}

template <typename ResT>
ResT GlobalFunction<ResT>::GetValue()const{
    return m_value;
}


template <typename ResT , typename ParamT>
GlobalFunction1<ResT , ParamT>::GlobalFunction1(ResT (&a_function) (ParamT) , ParamT a_param)
:m_function(a_function),
m_param(a_param){}


template <typename ResT , typename ParamT>
void GlobalFunction1<ResT , ParamT>::Run(){
    m_value = m_function(m_param);
}

template <typename ResT , typename ParamT>
ResT GlobalFunction1<ResT , ParamT>::GetValue()const{
    return m_value;
}

template <typename Class>
MemberActionFunction<Class>::MemberActionFunction(void(Class::* a_function)() , Class & a_this)
:m_function(a_function),
m_this(&a_this){}

template <typename Class>
void MemberActionFunction<Class>::Run(){
    (m_this->*m_function)();
}

template <typename ResT , typename Class , typename  Param>
MemberFunction1<ResT,Class,Param>::MemberFunction1(ResT(Class::* a_function)(Param),
 Class & a_this , Param a_param)
:m_function(a_function),
m_this(& a_this),
m_param(a_param){}

template <typename ResT , typename Class , typename  Param>
void MemberFunction1<ResT,Class,Param>::Run(){
    m_value = (m_this->*m_function)(m_param);
}

template <typename ResT , typename Class , typename  Param1 ,typename Param2>
MemberFunction2<ResT,Class,Param1,Param2>::MemberFunction2(ResT(Class::* a_function)(Param1 , Param2), Class & a_this ,
Param1 a_param , Param2 a_param2)
:m_function(a_function),
m_this(& a_this),
m_param(a_param),
m_param2(a_param2){}

template <typename ResT , typename Class , typename  Param1 ,typename Param2>
void MemberFunction2<ResT,Class,Param1,Param2>::Run(){
    m_value = (m_this->*m_function)(m_param , m_param2);
}

template <typename T>
Consumer<T>::Consumer(BlockingBoundedQueue<T> &a_queue , size_t a_size)
:m_queue(a_queue),
m_size(a_size){
    m_vector.reserve(a_size);
}

template <typename T>
void Consumer<T>::Run(){
    for(size_t i = 0 ; i < m_size ; ++i){
        T removed;
        m_queue.Dequeue(removed);
        m_vector.push_back(removed);
    }
}

template <typename T>
std::vector<T>const & Consumer<T>::GetVector()const{
    return m_vector;
}

template <typename T>
Producer<T>::Producer(BlockingBoundedQueue<T> &a_queue , size_t a_size , T a_item)
:m_queue(a_queue),
m_size(a_size),
m_item(a_item){
    m_vector.reserve(a_size);
}

template <typename T>
void Producer<T>::Run(){
    for(size_t i = 0; i < m_size ; ++i , ++m_item){
        m_queue.Enqueue(m_item);
        m_vector.push_back(m_item);
    }
}

template <typename T>
std::vector<T>const & Producer<T>::GetVector()const{
    return m_vector;
}

template <typename T>
ProducerWithVec<T>::ProducerWithVec(BlockingBoundedQueue<T> &a_queue, std::vector<T> &a_pool ,
 std::mutex &a_mutex , size_t a_size)
:m_queue(a_queue),
m_mutex(a_mutex),
m_pool(a_pool),
m_size(a_size){}

template <typename T>
void ProducerWithVec<T>::Run(){
    for(size_t i = 0; i < m_size ; ++i){
        m_mutex.lock();
        T removed = m_pool.back();
        m_pool.pop_back();
        m_queue.Enqueue(removed);
        m_vector.push_back(removed);
        m_mutex.unlock();
    }
}

template <typename T>
std::vector<T>const & ProducerWithVec<T>::GetVector()const{
    return m_vector;
}


template<typename T>
NewProducer<T>::NewProducer(BlockingBoundedQueue<T> &a_queue , size_t from , size_t to)
:m_queue(a_queue),
m_from(from),
m_to(to){}

template<typename T>
void NewProducer<T>::Run(){
    for (size_t i = m_from; i <= m_to; ++i){
        m_queue.Enqueue(i);
    }
}

template<typename T>
NewConsumer<T>::NewConsumer(BlockingBoundedQueue<T>& a_queue
,std::vector<std::pair<size_t, size_t>> const& a_categories, size_t a_size)
:m_queue(a_queue)
,m_categories(a_categories)
,m_expected(a_size){}

template<typename T>
void NewConsumer<T>::Run(){
    std::vector<size_t> previos;
    previos.reserve(m_categories.size());
    for(size_t i = 0; i < m_categories.size(); ++i){
        previos.push_back(m_categories[i].first);
    }
    for(size_t i = 0; i < m_expected ; ++i){
        m_isOk = true;
        T item;
        m_queue.Dequeue(item);
        auto itemId = FindRange(item);
        if(item == previos[itemId] + 1){
            previos[itemId] = item;
        }
        else{
            m_isOk = false;
            break;
        }
    }
}

template<typename T>
size_t NewConsumer<T>::FindRange(T a_item){
    for(size_t i = 0 ; i <m_categories.size() ; ++i){
        if(a_item >= m_categories[i].first && a_item <= m_categories[i].second){
            return i;
        }
    }
    return (m_categories.size() - 1);
}

template<typename T>
bool NewConsumer<T>::IsOk()const{
    return m_isOk;
}

#endif