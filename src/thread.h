#ifndef __RLEAVRS_THREAD_H__
#define __RLEAVRS_THREAD_H__

#include <memory>
#include "mutex.h"
#include <functional>


namespace rleavrs {

class Thread {

public:
    typedef std::shared_ptr<Thread> ptr;

    Thread(std::function<void()> cb, const std::string& name);
    ~Thread();
    pid_t getId() const;
    const std::string& getName() const;
    void join();
    static Thread* GetThis();
    static std::string GetName();
    static void SetName(const std::string& name);
    
private:
    static void* run(void* arg);

private:

    pid_t m_id = -1;
    pthread_t m_thread = 0;
    std::function<void()> m_cb;
    std::string m_name;
    Semaphore m_semaphore;

};

}

#endif