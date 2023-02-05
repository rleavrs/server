#ifndef __RLEAVRS__MUTEX_H__
#define __RLEAVRS__MUTEX_H__

#include <semaphore.h>
#include <stdexcept>
#include <pthread.h>

namespace rleavrs {

class Semaphore {
public:
    explicit Semaphore(uint count = 0);
    
    ~Semaphore();
    
    void wait();
    
    void notify();

private:
    sem_t m_semaphore;
};


template<class T>
class ScopeLoackImpl {  
public:
    ScopeLoackImpl(T& mutex)
        :m_mutex(mutex) {
        m_mutex.lock();
        m_locked = true;
        }

    ~ScopeLoackImpl() {
        unlock();
    }


    void lock() { 
        if(!m_locked) {
            m_mutex.lock();
            m_locked = true;
        }
    }

    void unlock()  {
        if(m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    T& m_mutex;
    bool m_locked;

};


class SpinLock {
public:
    typedef ScopeLoackImpl<SpinLock> Lock;
    
    SpinLock() {
        pthread_spin_init(&m_mutex,0);
    }
    
    ~SpinLock() {
        pthread_spin_destroy(&m_mutex);
    }
    
    void lock() {
        pthread_spin_lock(&m_mutex);
    }

    void unlock() {
        pthread_spin_unlock(&m_mutex);
    }
private:
    pthread_spinlock_t m_mutex;

};









}

#endif