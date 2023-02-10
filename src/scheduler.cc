#include "scheduler.h"


namespace rleavrs {

static Logger::ptr g_logger = RLEAVRS_LOG_NAME("system");

static thread_local Scheduler* t_scheduler = nullptr;
static thread_local Fiber* t_scheduler_fiber = nullptr;

Scheduler::Scheduler(size_t threads, bool use_caller, const std::string& name)
    :m_name(name) {
    RLEAVRS_ASSERT(threads > 0);
    if(use_caller){
        RLEAVRS_ASSERT(GetThis() == nullptr);
        threads--;
        t_scheduler = this;
        
        m_rootFiber.reset(new Fiber(std::bind(&Scheduler::run, this),0,true));
        Thread::SetName(m_name);
        
        t_scheduler_fiber = m_rootFiber.get();
        m_rootThread = GetThreadId();
        m_threadIds.push_back(m_rootThread);

    } else {
        m_rootThread = -1;
    }

    m_threadCounts = threads;
}

Scheduler::~Scheduler() {
    RLEAVRS_ASSERT(m_stopping);
    if(GetThis() == this) {
        t_scheduler = nullptr;
    }
}

void Scheduler::tickle() {
    RLEAVRS_LOG_INFO(g_logger) << "tickle";
}

void Scheduler::idle() {
    RLEAVRS_LOG_DEBUG(g_logger) << "this is Thread "<< GetThreadId() <<" idle func" << std::endl;
    while(!stopping()) {
        Fiber::YieldToHold();
    }
}

const std::string& Scheduler::getName() const {
    return m_name;
}

void Scheduler::start() {
    MutexType::Lock lock(m_mutex);
    if(!m_stopping) return;
    m_stopping = false;
    RLEAVRS_ASSERT(m_threads.empty());

    m_threads.resize(m_threadCounts);
    for(size_t i = 0; i < m_threadCounts; i++) {
        m_threads[i].reset(new Thread(std::bind(&Scheduler::run,this),m_name + "_" + std::to_string(i)));
        m_threadIds.push_back(m_threads[i]->getId());     
    }
}

void Scheduler::stop() {
    m_autoStop = true;
    if(m_rootFiber
            && m_threadCounts == 0
            && ((m_rootFiber->getState() == Fiber::TERM)
                ||  m_rootFiber->getState() == Fiber::INIT)){
        RLEAVRS_LOG_INFO(g_logger) << this << " stopped";
        m_stopping = true;
        if(stopping())  return;
    }
    
    if(m_rootThread != -1) {
        RLEAVRS_ASSERT(GetThis() == this);
    } else {
        RLEAVRS_ASSERT(GetThis() != this);
    }
    
    m_stopping = true;
    
    if(m_rootFiber) {
        if(!stopping()) {
            m_rootFiber->threadCall();
        }
    }

    std::vector<Thread::ptr> thrs;
    {
        MutexType::Lock lock(m_mutex);
        thrs.swap(m_threads);
    }

    for(auto& i : thrs) 
    {
        i->join();
    }  // wait for the end of all threads
}

void Scheduler::run() {
    RLEAVRS_LOG_DEBUG(g_logger) << "The Thread " << GetThreadId() << " scheduler is running" << std::endl;
    setThis();
    if(GetThreadId() != m_rootThread) {
        t_scheduler_fiber = Fiber::GetThis().get();
    }
    Fiber::ptr idle_fiber(new Fiber(std::bind(&Scheduler::idle,this)));
    Fiber::ptr cb_fiber;
    FiberAndThread ft;

    while(true) {
        // RLEAVRS_LOG_DEBUG(g_logger) << "activate thread" << m_activateThreadCount << std::endl;
        ft.reset();
        bool tickle_me = false;
        bool is_activate = false;
        {
            MutexType::Lock lock(m_mutex);
            auto it = m_fibers.begin();
            while(it != m_fibers.end()) {
                if(it->thread != -1 && it->thread != GetThreadId()) {
                    ++it;
                    tickle_me = true;
                    continue;
                }
                
                RLEAVRS_ASSERT(it->fiber || it->cb);
                if(it->fiber && it->fiber->getState() == Fiber::EXEC) {
                    ++it;
                    continue;
                }

                ft = *it;
                m_fibers.erase(it++);
                m_activateThreadCount++;
                is_activate = true;
                break;
            }
            tickle_me |= it != m_fibers.end();  
        }

        if(tickle_me)   tickle();
        
        if(ft.fiber && ft.fiber->getState() != Fiber::TERM 
                    && ft.fiber->getState() != Fiber::EXCEPT) {
            ft.fiber->fiberSwapIn();
            --m_activateThreadCount;
            
            if(ft.fiber->getState() == Fiber::READY) {
                schedule(ft.fiber);
            } else if(ft.fiber->getState() != Fiber::TERM
                        && ft.fiber->getState() != Fiber::EXCEPT) {
                ft.fiber->m_state = Fiber::HOLD;
            }
            ft.reset();
            } else if(ft.cb) {
                if(cb_fiber) {
                    cb_fiber->reset(ft.cb);
                } else {
                    cb_fiber.reset(new Fiber(ft.cb));
                }
                ft.reset();
                cb_fiber->fiberSwapIn();
                --m_activateThreadCount;

                if(cb_fiber->getState() == Fiber::READY) {
                    schedule(cb_fiber);
                    cb_fiber.reset();
                }  else if(cb_fiber->getState() == Fiber::EXCEPT
                            ||  cb_fiber->getState() == Fiber::TERM) {
                    cb_fiber->reset(nullptr);
                } else {
                    cb_fiber->m_state = Fiber::HOLD;
                    cb_fiber.reset();
                }

            } else {
            if(is_activate) {
                --m_activateThreadCount;
                continue;
            }
            if(idle_fiber->getState() == Fiber::TERM) {
                RLEAVRS_LOG_INFO(g_logger) << "idle fiber term";
                break;
            }

            idle_fiber->fiberSwapIn();
            if(idle_fiber->getState() != Fiber::TERM
                    && idle_fiber->getState() != Fiber::EXCEPT) {
                idle_fiber->m_state = Fiber::HOLD;
            }

        }
    }


}

bool Scheduler::stopping() {
    MutexType::Lock lock(m_mutex);
    return m_autoStop && m_stopping 
            && m_fibers.empty() 
            && m_activateThreadCount == 0;
}

void Scheduler::setThis() {
    t_scheduler = this;
}

bool Scheduler::hasIdleThread() {
    return m_idleThreadCount > 0;
}

Scheduler* Scheduler::GetThis() {
    return t_scheduler;
}

Fiber* Scheduler::GetMainFiber() {
    return t_scheduler_fiber;
}

void Scheduler::switchTo(int thread) {
    if(Scheduler::GetThis() == this) {
        if(thread == -1 || thread == GetThreadId()) {
            return;
        }    
    }

    schedule(Fiber::GetThis(), thread);
    Fiber::YieldToHold();
}





}