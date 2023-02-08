#ifndef __RLEAVRS__MUTEX_H__
#define __RLEAVRS__MUTEX_H__

#include <semaphore.h>
#include <stdexcept>
#include <pthread.h>
#include <atomic>

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

template<class T>
class ReadScopeLockImpl {
public:
    ReadScopeLockImpl(T& mutex)
        :m_mutex(mutex) {
        m_mutex.rdlock();
        m_lockde = true;
    }

    ~ReadScopeLockImpl() {
        unlock();
    }

    void lock() {
        if(!m_lockde) {
            m_mutex.rdlock();
            m_lockde = true;
        }
    }

    void unlock()  {
        if(m_lockde) {
            m_mutex.unlock();
            m_lockde = false;
        }
    }

private:
    T& m_mutex;
    bool m_lockde;
};

template <class T>
class  WriteScopeLockImpl {
public:
    WriteScopeLockImpl(T& mutex):
        m_mutex(mutex) {
            m_mutex.wrlock();
            m_locked = true;
        }

    ~WriteScopeLockImpl() {
        m_mutex.unlock();
    }


    void lock() {
        if(!m_locked) {
            m_mutex.wrlock();
            m_locked = true;
        }
    }
    
    void unlock() {
        if(!m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    bool m_locked;
    T&  m_mutex;

};

class Mutex {
public:
    typedef ScopeLoackImpl<Mutex> Lock;

    Mutex () {
        pthread_mutex_init(&m_mutex,nullptr);        
    }  

    ~Mutex() {
        pthread_mutex_destroy(&m_mutex);
    }

    void lock () {
        pthread_mutex_lock(&m_mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&m_mutex);
    }
private:

    pthread_mutex_t m_mutex;

};


class RWMutex  {
public:
    typedef ReadScopeLockImpl<RWMutex> ReadLock;
    typedef WriteScopeLockImpl<RWMutex> WriteLock;
    
    RWMutex() {
        pthread_rwlock_init(&m_lock, nullptr);
    }

    ~RWMutex() {
        pthread_rwlock_destroy(&m_lock);
    }

    void rdlock() {
        pthread_rwlock_rdlock(&m_lock);
    }   

    void wrlock() {
        pthread_rwlock_wrlock(&m_lock);
    }
    
    void unlock() {
        pthread_rwlock_unlock(&m_lock);
    }

private:
    pthread_rwlock_t m_lock;

};



class Spinlock {
public:
    typedef ScopeLoackImpl<Spinlock> Lock;
    
    Spinlock() {
        pthread_spin_init(&m_mutex,0);
    }
    
    ~Spinlock() {
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

class CASLock {
public:
    typedef ScopeLoackImpl<CASLock> Lock;

    CASLock() {
        m_mutex.clear();
    }

    ~CASLock() {}
    
    void lock () {
        while(std::atomic_flag_test_and_set_explicit(&m_mutex,std::memory_order_acquire)); 
    }

    void unlock() {
        std::atomic_flag_clear_explicit(&m_mutex,std::memory_order_release);
    }

private:
    volatile std::atomic_flag m_mutex;

};

}

#endif