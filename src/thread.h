#ifndef __RLEAVRS_THREAD_H__
#define __RLEAVRS_THREAD_H__

#include <thread>
#include <pthread.h>
#include <functional>
#include <memory>
#include <string>

// thread + 
// mutex - <pthread>
// C++11- pthread_xxx
// std::stread,pthread
// xie cheng

namespace rleavrs {

class Thread {
public:
    typedef std::shared_ptr<Thread> ptr;

    Thread(std::function<void()> cb, const std::string name);
    ~Thread();

    const std::string& getName() const;
    // getpid();
    // pthread_self();
    const tid_t getId() const ;
    void join();
    static Thread* GetThis();
    static const std::string& GetName();
    

    
private:
    Thread(const Thread&) = delete;
    Thread(const Thread&&) = delete;
    Thread& operator=(const Thread&) = delete;
    
public:
    tid_t m_id;
    pthread_t m_thread;
    std::function<void()> m_cb;
    std::string m_name;
};
}



#endif