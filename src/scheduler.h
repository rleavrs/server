#ifndef __RLEAVRS_SCHEDULER_H__
#define __RLEAVRS_SCHEDULER_H__

#include <memory>
#include "thread.h"
#include <vector>
#include <list>
#include "fiber.h"


namespace rleavrs {

class Scheduler {
public:
    typedef std::shared_ptr<Scheduler> ptr;
    typedef Mutex MutexType;

    Scheduler();
    virtual ~Scheduler() = 0;

    const std::string& getName() const;
    


private:
    class FiberAndThread {
        
    };


private:
    MutexType m_mutex;
    std::vector<Thread::ptr> m_threads;
    std::list<FiberAndThread> m_fibers;
    Fiber::ptr m_rootFiber;
    std::string m_name;

private:

    std::vector<int> m_threadIds;
    size_t m_threadCounts;
    std::atomic<size_t> m_activateThreadCount = {0};
    std::atomic<size_t> m_idleThreadCount = {0};
    bool m_stopping = true;
    bool m_autoStop = false;
    int m_rootThread = 0;




};


}



#endif