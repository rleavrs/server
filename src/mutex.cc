#include "mutex.h"

namespace rleavrs {

Semaphore::Semaphore(uint count) {
    if(sem_init(&m_semaphore,0,count)) {
        throw std::logic_error("sem_init error");   
    }
}
    
Semaphore::~Semaphore() {
    if(sem_destroy(&m_semaphore)) {
        throw std::logic_error("sem_destroy error");
    }
}
    
void Semaphore::wait() {
    if(sem_wait(&m_semaphore)) {
        throw std::logic_error("sem_wait error");
    }
}

void Semaphore::notify() {
    if(sem_post(&m_semaphore)) {
        throw std::logic_error("sem_notify error");
    }
}

}