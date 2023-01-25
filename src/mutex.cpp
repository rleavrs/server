#include "mutex.h"

namespace rleavrs {

Semaphore::Semaphore(u_int32_t count) {
    if(sem_init(&m_semaphore, 0, count)){
        throw std::logic_error("sem_init error");
    }
}
Semaphore::~Semaphore() {
    sem_destroy(&m_semaphore);
}
void Semaphore::wait() {
    if(sem_wait(&m_semaphore)) {
        throw std::logic_error("sem_wait error");
    }
}
void Semaphore::notify() {
    if(sem_post(&m_semaphore)) {
        throw std::logic_error("sem_post error");
    }
}






Mutex::Mutex() {
    pthread_mutex_init(&m_mutex, nullptr);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&m_mutex);
}

void Mutex::lock() {
    pthread_mutex_lock(&m_mutex);
}

void Mutex::unlock() {
    pthread_mutex_unlock(&m_mutex);
}






}


