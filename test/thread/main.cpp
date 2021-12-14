#include "mu_test.h"
#include <iostream>
#include <unistd.h>
#include "counter.hpp"
#include "funcadapter.hpp"
#include "thread_group.hpp"

using namespace std;
using namespace advcpp;

int g_x;
int g_u;
int g_count;

double y_x;

void g_FuncNoResNoArgsWithSleep()
{
    sleep(5);
    ++g_count;
}

int test(){
    ++g_x;
    return g_x;
}

double tes2Params(int size){
    for(int i = 0 ; i< size ; ++i){
        ++y_x;
    }
    return y_x;
}

void voidTest(){
    ++g_u;
    //sleep(1);
}


BEGIN_TEST(test_global_func)
    std::shared_ptr<Callable> CallablePtr(new GlobalFunction<int>(test));
    Thread t(CallablePtr , Thread::JOIN);
    t.Join();
    ASSERT_THAT(g_x == 1);

    AtomicValue<int> a (42);

END_TEST

BEGIN_TEST(global_2_params)
    int size = 5;
    GlobalFunction1<double , int> * adaptor = new GlobalFunction1<double , int>(tes2Params , size);
    std::shared_ptr<Callable> CallablePtr(adaptor);
    Thread t(CallablePtr, Thread::JOIN);
    t.Join();

    ASSERT_EQUAL(y_x , 5);
    ASSERT_EQUAL(adaptor->GetValue() , 5);

END_TEST


BEGIN_TEST(void_global)
    std::shared_ptr<Callable> CallablePtr(new ActionFunction<void>(voidTest));
    Thread t(CallablePtr, Thread::JOIN);
    t.Join();

    ASSERT_EQUAL(g_u , 1);


END_TEST

BEGIN_TEST(member_function)
    typedef MemberActionFunction<Counter> adapter;
    Counter count(10);
    std::shared_ptr<Callable> CallablePtr(new adapter(&Counter::Increment , count));
    Thread t(CallablePtr, Thread::JOIN);
    t.Join();

    ASSERT_EQUAL(count.GetCount() , 11);

END_TEST


BEGIN_TEST(two_threads_10_times_inc)
    typedef MemberFunction1<size_t, Counter , size_t> adapter;
    Counter count(0);
    size_t size = 10;
    std::shared_ptr<Callable> CallablePtr(new adapter(&Counter::IncrementLoop , count , size));
    Thread t(CallablePtr, Thread::JOIN);
    Thread t2(CallablePtr, Thread::JOIN);

    t.Join();
    t2.Join();

    ASSERT_EQUAL(count.GetCount() , 20);    

END_TEST

BEGIN_TEST(two_threads_10000_times_inc)
    typedef MemberFunction1<size_t, Counter , size_t> adapter;
    Counter count(0);
    size_t size = 10000;
    std::shared_ptr<Callable> CallablePtr(new adapter(&Counter::IncrementLoop , count , size));
    Thread t(CallablePtr, Thread::JOIN);
    Thread t2(CallablePtr, Thread::JOIN);

    t.Join();
    t2.Join();

    ASSERT_EQUAL(count.GetCount() , 20000);    

END_TEST

BEGIN_TEST(two_threads_100000_times_inc)
    typedef MemberFunction1<size_t, Counter , size_t> adapter;
    Counter count(0);
    size_t size = 100000;
    std::shared_ptr<Callable> CallablePtr(new adapter(&Counter::IncrementLoop , count , size));
    Thread t(CallablePtr, Thread::JOIN);
    Thread t2(CallablePtr, Thread::JOIN);

    t.Join();
    t2.Join();

    ASSERT_EQUAL(count.GetCount() , 200000);    

END_TEST

BEGIN_TEST(test_group)
    typedef MemberFunction1<size_t, Counter , size_t> adapter;
    Counter count(0);
    size_t size = 100000;
    std::shared_ptr<Callable> CallablePtr(new adapter(&Counter::IncrementLoop , count , size));
    ThreadGroup threadG(3 , CallablePtr);

    threadG.JoinAll();

    ASSERT_EQUAL(count.GetCount() , 300000);    

END_TEST


BEGIN_TEST(one_inc_one_dec_10_times)
    typedef MemberFunction1<size_t, Counter , size_t> adaperFunc;
    Counter count(100);
    size_t size = 10;
    adaperFunc * adaptorInc = new adaperFunc(&Counter::IncrementLoop , count , size);
    adaperFunc * adaptorDec = new adaperFunc(&Counter::DecrementLoop , count , size);
    std::shared_ptr<Callable> CallablePtrInc(adaptorInc);
    std::shared_ptr<Callable> CallablePtrDec(adaptorDec);
    Thread t(CallablePtrInc, Thread::JOIN);
    Thread t2(CallablePtrDec, Thread::JOIN);
    t.Join();
    t2.Join();

    ASSERT_EQUAL(count.GetCount() , 100);
    TRACE(adaptorInc->GetValue());
    TRACE(adaptorDec->GetValue());


END_TEST

BEGIN_TEST(one_inc_one_dec_10000_times)
    typedef MemberFunction1<size_t, Counter , size_t> adaperFunc;
    Counter count(50000);
    size_t size = 10000;
    adaperFunc * adaptorInc = new adaperFunc(&Counter::IncrementLoop , count , size);
    adaperFunc * adaptorDec = new adaperFunc(&Counter::DecrementLoop , count , size);
    std::shared_ptr<Callable> CallablePtrInc(adaptorInc);
    std::shared_ptr<Callable> CallablePtrDec(adaptorDec);
    Thread t(CallablePtrInc, Thread::JOIN);
    Thread t2(CallablePtrDec, Thread::JOIN);
    t.Join();
    t2.Join();

    ASSERT_EQUAL(count.GetCount() , 50000);
    TRACE(adaptorInc->GetValue());
    TRACE(adaptorDec->GetValue());


END_TEST

BEGIN_TEST(one_inc_one_dec_100000_times)
    typedef MemberFunction1<size_t, Counter , size_t> adaperFunc;
    Counter count(500000);
    size_t size = 100000;
    adaperFunc * adaptorInc = new adaperFunc(&Counter::IncrementLoop , count , size);
    adaperFunc * adaptorDec = new adaperFunc(&Counter::DecrementLoop , count , size);
    std::shared_ptr<Callable> CallablePtrInc(adaptorInc);
    std::shared_ptr<Callable> CallablePtrDec(adaptorDec);
    Thread t(CallablePtrInc, Thread::JOIN);
    Thread t2(CallablePtrDec, Thread::JOIN);
    t.Join();
    t2.Join();

    ASSERT_EQUAL(count.GetCount() , 500000);
    TRACE(adaptorInc->GetValue());
    TRACE(adaptorDec->GetValue());


END_TEST

BEGIN_TEST(shred_ptr_counter)
    typedef MemberFunction1<size_t, Counter , size_t> adapter;
    Counter count(0);
    size_t size = 10;//FIXME: race condition on 10K
    std::shared_ptr<Callable> CallablePtr(new adapter(&Counter::IncrementLoop , count , size));
    ASSERT_EQUAL(CallablePtr.use_count() , 1);
{
    Thread t(CallablePtr, Thread::JOIN);
    t.Detach();
}
    ASSERT_EQUAL(CallablePtr.use_count() , 2);

END_TEST

BEGIN_TEST(group_add)
    typedef MemberFunction1<size_t, Counter , size_t> adapter;
    Counter count(0);
    size_t size = 100;
    std::shared_ptr<Callable> CallablePtr(new adapter(&Counter::IncrementLoop , count , size));
    ThreadGroup threadG(3 , CallablePtr);
    threadG.Add(2);
    ASSERT_EQUAL(threadG.GetNumberOfThreads() , 5);

    threadG.JoinAll();

    ASSERT_EQUAL(count.GetCount() , 500);    


END_TEST

BEGIN_TEST(remove_group)
    typedef MemberFunction1<size_t, Counter , size_t> adapter;
    Counter count(0);
    size_t size = 100;
    std::shared_ptr<Callable> CallablePtr(new adapter(&Counter::IncrementLoop , count , size));
    ThreadGroup threadG(5 , CallablePtr);
    threadG.Remove(2);
    ASSERT_EQUAL(threadG.GetNumberOfThreads() , 3);

    threadG.JoinAll();

END_TEST

BEGIN_TEST(test_2_join)
typedef MemberActionFunction<Counter> adapter;
    Counter count(10);
    std::shared_ptr<Callable> CallablePtr(new adapter(&Counter::Increment , count));
    Thread t(CallablePtr, Thread::JOIN);
    t.Join();
    t.Join();

    ASSERT_EQUAL(count.GetCount() , 11);

END_TEST


BEGIN_SUITE(Thread)
    TEST(test_global_func)
    TEST(global_2_params)
    TEST(void_global)
    TEST(member_function)
    TEST(two_threads_10_times_inc)
    TEST(two_threads_10000_times_inc)
    TEST(two_threads_100000_times_inc)
    TEST(test_group)
    TEST(one_inc_one_dec_10_times)
    TEST(one_inc_one_dec_10000_times)
    TEST(one_inc_one_dec_100000_times)
    TEST(shred_ptr_counter)
    TEST(group_add)
    TEST(remove_group)
    TEST(test_2_join)

END_SUITE
