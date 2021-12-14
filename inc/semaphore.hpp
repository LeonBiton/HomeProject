#include <semaphore.h>

namespace advcpp{
class Semaphore{
public:
    Semaphore(int a_pshared = 0 , unsigned int a_semVal = 0);
    ~Semaphore();

    int Wait();
    int Post();
    int GetValue(int * a_sval);

private:
    sem_t m_semaphore;

};
}