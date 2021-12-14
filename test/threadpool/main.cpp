#include <unistd.h>

#include <iostream>
#include <vector>

#include "mu_test.h"
#include "threadpool.hpp"

using namespace std;
using namespace advcpp;

class Test : public Callable {
   public:
    Test() : m_counter(0), m_mutex() {}

    void Run() {
        Guard g(m_mutex);
        ++m_counter;
    }

    size_t GetCounter() const { return m_counter; }

   private:
    size_t m_counter;
    std::mutex m_mutex;
};

class ReportingTask : public Callable{
public:
    virtual bool IsDone() = 0;

};

class Task : public ReportingTask {
    Semaphore sem;

public:
    bool IsDone()override{
        sem.Wait();
        return true;
    }

    void Run() {
        sem.Post();
    }
};

class WTFTask : public ReportingTask{
    Semaphore sem;

public:
    bool IsDone(){
        sem.Wait();
        return true;
    }
    void Run() override
    {
        sem.Post();
        throw std::runtime_error("exception in task wtf!");
    }
};

BEGIN_TEST(insert_to_threadpool)
ThreadPool pool(100, 0);
std::shared_ptr<Test> test(new Test);
for (int i = 0; i < 100; ++i) {
    pool.Submit(test);
}
ASSERT_EQUAL(pool.TasksInProgress(), 100);

END_TEST

BEGIN_TEST(tp_run_test)

    ThreadPool pool(42, 1);
    std::shared_ptr<Task> test(new Task());
    pool.Submit(test);
    ASSERT_THAT(test->IsDone());
    ASSERT_EQUAL(pool.NumberOfThreads() , 1);
    ASSERT_EQUAL(pool.TasksInProgress() , 0);


END_TEST

BEGIN_TEST(test_with_exception)
    ThreadPool pool(42, 1);
    std::shared_ptr<WTFTask> task(new WTFTask);
    pool.Submit(task);
    ASSERT_THAT(task->IsDone());
    ASSERT_EQUAL(pool.NumberOfThreads() , 1);
    ASSERT_EQUAL(pool.TasksInProgress() , 0);

END_TEST

BEGIN_TEST(test_with_exception_and_normal_task)
    ThreadPool pool(42, 1);
    std::shared_ptr<WTFTask> wtftask(new WTFTask());
    pool.Submit(wtftask);

    std::shared_ptr<Task> task(new Task());
    pool.Submit(task);

    ASSERT_THAT(wtftask->IsDone());
    ASSERT_THAT(task->IsDone());

    ASSERT_EQUAL(pool.NumberOfThreads() , 1);
    ASSERT_EQUAL(pool.TasksInProgress() , 0);

END_TEST

BEGIN_TEST(tp_many_tasks)
    using std::vector;
    using std::shared_ptr;
    ThreadPool pool(25, 1);

    vector<shared_ptr<ReportingTask>> tasks;
    const int n = 50;
    tasks.reserve(n);
    for(int i = 0 ; i< n ; ++++i)
    {
        tasks.emplace_back(new WTFTask());
        pool.Submit(tasks.back());
        tasks.emplace_back(new Task());
        pool.Submit(tasks.back());
    }
    for(int i = 0 ; i < n ; ++i)   
    {
        ASSERT_THAT(tasks[i]->IsDone());
    }    
    ASSERT_EQUAL(pool.TasksInProgress() , 0);
    ASSERT_EQUAL(pool.NumberOfThreads() , 1);

END_TEST

BEGIN_TEST(shutdown_immediate)
    ThreadPool pool(25, 1);

    vector<shared_ptr<ReportingTask>> tasks;
    const int n = 5000;
    tasks.reserve(n);
    for(int i = 0 ; i< n ; ++++i)
    {
        tasks.emplace_back(new WTFTask());
        pool.Submit(tasks.back());
        tasks.emplace_back(new Task());
        pool.Submit(tasks.back());
    }
    pool.ShutdownImmediate();
    ASSERT_THAT(pool.TasksInProgress() > 0);

END_TEST

BEGIN_TEST(zero_threads_then_add_one)
    ThreadPool pool(25, 0);

    ASSERT_EQUAL(pool.TasksInProgress() , 0);
    ASSERT_EQUAL(pool.NumberOfThreads() , 0);

    std::shared_ptr<Task> task(new Task());
    pool.Submit(task);
    ASSERT_EQUAL(pool.TasksInProgress() , 1);
    ASSERT_EQUAL(pool.NumberOfThreads() , 0);
    pool.Add(1);
    ASSERT_THAT(task->IsDone());
    ASSERT_EQUAL(pool.TasksInProgress() , 0);
    ASSERT_EQUAL(pool.NumberOfThreads() , 1);

END_TEST

BEGIN_TEST(build_and_submit)
    ThreadPool pool(100, 10);
    std::shared_ptr<Test> test(new Test);
    for (size_t i = 0; i < 10000; ++i) {
        pool.Submit(test);
    }
    pool.Shutdown();

ASSERT_EQUAL(test->GetCounter(), 10000);

END_TEST

BEGIN_TEST(test_add)
    ThreadPool pool(100, 10);
    std::shared_ptr<Test> test(new Test);
    pool.Add(5);
    for (size_t i = 0; i < 10000; ++i) {
        pool.Submit(test);
    }
    pool.Shutdown();

ASSERT_EQUAL(pool.NumberOfThreads(), 15);

END_TEST

BEGIN_TEST(test_remove)
    ThreadPool pool(1000, 4);
    ASSERT_EQUAL(pool.NumberOfThreads() , 4);
    pool.Remove(1);
    ASSERT_EQUAL(pool.NumberOfThreads() , 3);

END_TEST

BEGIN_TEST(remove_and_add)
    ThreadPool pool(50, 1);
    pool.Remove(1);
    ASSERT_EQUAL(pool.TasksInProgress() , 0);
    ASSERT_EQUAL(pool.NumberOfThreads() , 0);
    std::shared_ptr<Task> task(new Task());
    pool.Submit(task);
    ASSERT_EQUAL(pool.TasksInProgress() , 1);
    ASSERT_EQUAL(pool.NumberOfThreads() , 0);
    pool.Add(1);
    ASSERT_THAT(task->IsDone());
    ASSERT_EQUAL(pool.TasksInProgress() , 0);
    ASSERT_EQUAL(pool.NumberOfThreads() , 1);


END_TEST



BEGIN_SUITE(ThreadPool)
    TEST(insert_to_threadpool)
    TEST(tp_run_test)
    TEST(test_with_exception)
    TEST(test_with_exception_and_normal_task)
    TEST(tp_many_tasks)
    TEST(zero_threads_then_add_one)
    TEST(build_and_submit)
    TEST(test_add)
    TEST(test_remove)
    TEST(remove_and_add)
    TEST(shutdown_immediate)

END_SUITE
