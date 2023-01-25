#ifndef __RLEAVRS_MUTEX_H__
#define __RLEAVRS_MUTEX_H__

#include "noncopyable.h"
#include <semaphore.h>
#include <stdexcept>
#include <mutex>


// int sem_init(sem_t *sem,int pshared,unsigned int value);
// int sem_destroy(sem_t *sem); 
// int sem_wait(sem_t *sem); 
// int sem_trywait(sem_t *sem); 
// int sem_post(sem_t *sem); 
// int sem_getvalue(sem_t *sem);  

namespace rleavrs {

class Semaphore : public Noncopyable {
public:
    Semaphore(u_int32_t count = 0);
    ~Semaphore();
    void wait();
    void notify();
    
private:
    sem_t m_semaphore;
};

template<class T>
class ScopedLockImpl {
public:
    ScopedLockImpl(T& mutex):
        m_mutex(mutex),
        m_locked(false) {
        m_mutex.lock();
    }
    ~ScopedLockImpl() {
        unlock();
    }
    void lock() {
        if(!m_locked) {
            m_mutex.lock();
            m_locked();
        }
    }
    void unlock() {
        if(m_locked) {
            m_mutex.unlock();
        }
    }
private:
    T& m_mutex;
    bool m_locked;

};


template<class T>
class ReadScopedLockImpl {

};

template<class T>
class WriteScopedLockImpl {

};


class Mutex : public Noncopyable {
public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();

private:
    pthread_mutex_t m_mutex;
};

class NullMutex : Noncopyable {
public:
    NullMutex();

    

private:

};

class RWMutex : Noncopyable {
    
};

class spinlock : Noncopyable {

};

class CASLock : Noncopyable {

};



}

#endif 