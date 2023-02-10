#ifndef __RLEAVRS_SCHEDULER_H__
#define __RLEAVRS_SCHEDULER_H__

#include <memory>
#include "thread.h"
#include <vector>
#include <list>
#include "fiber.h"
#include "utility.h"


namespace rleavrs {

class Scheduler {
public:
    typedef std::shared_ptr<Scheduler> ptr;
    typedef Mutex MutexType;
    
    Scheduler(size_t threads = 1, bool use_caller = true, const std::string& name = "");
    virtual ~Scheduler();
    
    const std::string& getName() const;
    void start();
    void stop();    

    static Scheduler* GetThis();
    static Fiber* GetMainFiber();
    
protected:
    virtual void tickle();
    virtual void idle();
    virtual bool stopping();
    void setThis();
    bool hasIdleThread();
    void run();

private:
    class FiberAndThread {
    public:
        FiberAndThread(Fiber::ptr f, int th)
            :fiber(f), thread(th){}
        FiberAndThread(Fiber::ptr* f, int th)
            :thread(th) {
            fiber.swap(*f);      
        } 
        FiberAndThread(std::function<void()> f,int th)
            :cb(f),thread(th) {}
        FiberAndThread(std::function<void()>* f,int th)
            :thread(th) {
            cb.swap(*f);           
        }
        FiberAndThread(): thread(-1){}

        void reset () { fiber = nullptr , cb = nullptr, thread = -1;}
        int thread;
        Fiber::ptr fiber;
        std::function<void()> cb;
    };


private:
    
    template<class FOC>
    bool scheduleNoLock(FOC fc, int thread) {
        bool tickle = m_fibers.empty();

        FiberAndThread ft(fc, thread);
        if(ft.fiber || ft.cb) {
            m_fibers.push_back(ft);
            //std::cout << "Cur fiber " << m_fibers.size() << std::endl; 
        }

        return tickle;
    }
    
public:
    template<class FOC>
    bool schedule(FOC foc, int thread = -1) {
        bool tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            tickle = scheduleNoLock(foc, thread);
        }
        return tickle;
    }

    template<class FOCIterator>
    bool schedule(FOCIterator begin, FOCIterator end) {
        bool tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            while(begin != end) {
                tickle = scheduleNoLock(&*begin,-1) || tickle;  // why upload address?
                begin++;
            }     
        }
        return tickle;
    }
    

protected:
    std::vector<int> m_threadIds;
    size_t m_threadCounts;
    std::atomic<size_t> m_activateThreadCount = {0};
    std::atomic<size_t> m_idleThreadCount = {0};
    bool m_stopping = true;
    bool m_autoStop = false;
    int m_rootThread = 0;


private:
    MutexType m_mutex;
    std::vector<Thread::ptr> m_threads;
    std::list<FiberAndThread> m_fibers;
    Fiber::ptr m_rootFiber;
    std::string m_name;

};


}



#endif