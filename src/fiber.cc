#include "fiber.h"
#include <atomic>
#include "utility.h"
#include "scheduler.h"

namespace rleavrs {

static Logger::ptr g_logger = RLEAVRS_LOG_NAME("system");
    
static std::atomic<uint64_t> s_fiber_id {0};
static std::atomic<uint64_t> s_fiber_count {0};

static thread_local Fiber* t_fiber = nullptr;
static thread_local Fiber::ptr t_threadFiber = nullptr;

class MallocStackAllocator {
public:
    static void* Alloc(size_t size) {
        return malloc(size);
    }
    
    static void Dealloc(void* vp, size_t size) {
        return free(vp);
    }
};

Fiber::Fiber() {
    m_state = EXEC;
    SetThis(this);

    if(getcontext(&m_ctx)) {
        RLEAVRS_ASSERT_W(false, "getcontext");
    }
    
    ++s_fiber_count;
}

Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool use_caller)
    :m_id(++s_fiber_id),
    m_cb(cb) {
    ++s_fiber_count;
    // m_stacksize = stacksize ? stacksize : 128 * 1024; // cooperating with config system, but now use default value(128 * 1024K = 128M)
    m_stacksize = 128 * 1024;
    m_stack = MallocStackAllocator::Alloc(m_stacksize);
    
    if(getcontext(&m_ctx)) {
        RLEAVRS_ASSERT_W (false, "getcontext");
    }
    
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

    if(!use_caller) {
        makecontext(&m_ctx, &Fiber::MainFunc, 0);
    } else {
        makecontext(&m_ctx,&Fiber::CallerMainFunc,0);
    }
}

Fiber::~Fiber() {
    s_fiber_count--;
    if(m_stack) {
        RLEAVRS_ASSERT( m_state == TERM ||
                        m_state == INIT ||
                        m_state == EXCEPT);
        MallocStackAllocator::Dealloc(m_stack,m_stacksize);
    } else {
        RLEAVRS_ASSERT(!m_cb);
        RLEAVRS_ASSERT(m_state == EXEC);
        Fiber* cur = t_fiber;
        if(cur == this) SetThis(nullptr);
    }
    RLEAVRS_LOG_DEBUG(g_logger) << "FIber::~Fiber id = " << m_id << " tatal = " << s_fiber_count;
}

void Fiber::reset(std::function<void()> cb) {
    RLEAVRS_ASSERT(m_stack);
    RLEAVRS_ASSERT(m_state == TERM
                || m_state == EXCEPT
                || m_state == INIT);
    m_cb = cb;
    if(getcontext(&m_ctx)) {
        RLEAVRS_ASSERT_W(false, "getcontext");
    }
    
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;
    
    makecontext(&m_ctx, &Fiber::MainFunc, 0);
    m_state = INIT;

}

void Fiber::fiberSwapIn() {
    SetThis(this);
    RLEAVRS_ASSERT(m_state != EXEC);
    m_state = EXEC;
    if(swapcontext(&Scheduler::GetMainFiber()->m_ctx, &m_ctx)) {
        RLEAVRS_ASSERT_W(false, "swapcontext");
    }
}

void Fiber::fiberSwapOut() {
    SetThis(Scheduler::GetMainFiber());
    if(swapcontext(&m_ctx, &Scheduler::GetMainFiber()->m_ctx)) {
        RLEAVRS_ASSERT_W(false, "swapcontext");
    }
}

void Fiber::threadCall() {
    SetThis(this);
    m_state = EXEC;
    RLEAVRS_ASSERT(t_threadFiber);
    
    if(swapcontext(&t_threadFiber->m_ctx,&m_ctx)) {
        RLEAVRS_ASSERT_W(false, "swapcontext");
    }
    
}

void Fiber::threadBack() {
    SetThis(t_threadFiber.get());
    RLEAVRS_ASSERT(t_threadFiber);
    if(swapcontext(&m_ctx, &t_threadFiber->m_ctx)) {
        RLEAVRS_ASSERT_W(false, "swapcontext");
    }

}

void Fiber::SetThis(Fiber* f) {
    t_fiber = f;
}

Fiber::ptr Fiber::GetThis() {
    if(t_fiber) {
        return t_fiber->shared_from_this();
    }
    Fiber::ptr main_fiber(new Fiber);
    RLEAVRS_ASSERT(t_fiber == main_fiber.get());
    t_threadFiber = main_fiber;
    RLEAVRS_ASSERT(t_threadFiber);
    return t_fiber->shared_from_this();
}   

uint64_t Fiber::GetFiberId() {
    if(t_fiber) {
        return t_fiber->getId();
    }
    return 0;
}

void Fiber::YieldToReady() {
    Fiber::ptr cur = GetThis();
    RLEAVRS_ASSERT(cur->m_state == EXEC);
    cur -> m_state = READY;
    cur -> fiberSwapOut();
}

void Fiber::YieldToHold() {
    Fiber::ptr cur = GetThis();
    RLEAVRS_ASSERT(cur->m_state == EXEC);
    // cur -> m_state = HOLD;
    cur -> fiberSwapOut();
}

uint64_t Fiber::TotalFibers() {
    return s_fiber_count;
}

void Fiber::MainFunc() {
    Fiber::ptr cur = GetThis();
    try{
        cur->m_cb();
        cur->m_cb = nullptr;
        cur->m_state = TERM;    
    } catch(std::exception& e) {
        cur->m_state = EXCEPT;

    } catch(...) {
        cur->m_state = EXCEPT;   
    }
    
    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->fiberSwapOut();
}

void Fiber::CallerMainFunc() {
    Fiber::ptr cur = GetThis();
    try{
        cur->m_cb();
        cur->m_cb = nullptr;
        cur-> m_state = TERM;
    } catch(std::exception &e) {
        cur->m_state = EXCEPT;
    } catch(...) {
        cur->m_state = EXCEPT;
    }

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->threadBack();
}



}



// #include "fiber.h"
// #include "utility.h"
// #include "log.h"
// #include "scheduler.h"
// #include <atomic>

// namespace rleavrs {

// static Logger::ptr g_logger = RLEAVRS_LOG_NAME("system");

// static std::atomic<uint64_t> s_fiber_id {0};
// static std::atomic<uint64_t> s_fiber_count {0};

// static thread_local Fiber* t_fiber = nullptr;
// static thread_local Fiber::ptr t_threadFiber = nullptr;

// class MallocStackAllocator {
// public:
//     static void* Alloc(size_t size) {
//         return malloc(size);
//     }

//     static void Dealloc(void* vp, size_t size) {
//         return free(vp);
//     }
// };

// using StackAllocator = MallocStackAllocator;

// uint64_t Fiber::GetFiberId() {
//     if(t_fiber) {
//         return t_fiber->getId();
//     }
//     return 0;
// }

// Fiber::Fiber() {
//     m_state = EXEC;
//     SetThis(this);

//     if(getcontext(&m_ctx)) {
//         RLEAVRS_ASSERT_W(false, "getcontext");
//     }

//     ++s_fiber_count;
//     RLEAVRS_LOG_DEBUG(g_logger) << "Fiber::Fiber main";
// }

// Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool use_caller)
//     :m_id(++s_fiber_id)
//     ,m_cb(cb) {
//     ++s_fiber_count;

//     m_stacksize = 128 * 1024;

//     m_stack = StackAllocator::Alloc(m_stacksize);
//     if(getcontext(&m_ctx)) {
//         RLEAVRS_ASSERT_W(false, "getcontext");
//     }
//     m_ctx.uc_link = nullptr;
//     m_ctx.uc_stack.ss_sp = m_stack;
//     m_ctx.uc_stack.ss_size = m_stacksize;

//     if(!use_caller) {
//         makecontext(&m_ctx, &Fiber::MainFunc, 0);
//     } else {
//         makecontext(&m_ctx, &Fiber::CallerMainFunc, 0);
//     }

//     RLEAVRS_LOG_DEBUG(g_logger) << "Fiber::Fiber id=" << m_id;
// }

// Fiber::~Fiber() {
//     --s_fiber_count;
//     if(m_stack) {
//         RLEAVRS_ASSERT(m_state == TERM
//                 || m_state == EXCEPT
//                 || m_state == INIT);

//         StackAllocator::Dealloc(m_stack, m_stacksize);
//     } else {
//         RLEAVRS_ASSERT(!m_cb);
//         RLEAVRS_ASSERT(m_state == EXEC);

//         Fiber* cur = t_fiber;
//         if(cur == this) {
//             SetThis(nullptr);
//         }
//     }
//     RLEAVRS_LOG_DEBUG(g_logger) << "Fiber::~Fiber id=" << m_id
//                               << " total=" << s_fiber_count;
// }

// //重置协程函数，并重置状态
// //INIT，TERM, EXCEPT
// void Fiber::reset(std::function<void()> cb) {
//     RLEAVRS_ASSERT(m_stack);
//     RLEAVRS_ASSERT(m_state == TERM
//             || m_state == EXCEPT
//             || m_state == INIT);
//     m_cb = cb;
//     if(getcontext(&m_ctx)) {
//         RLEAVRS_ASSERT_W(false, "getcontext");
//     }

//     m_ctx.uc_link = nullptr;
//     m_ctx.uc_stack.ss_sp = m_stack;
//     m_ctx.uc_stack.ss_size = m_stacksize;

//     makecontext(&m_ctx, &Fiber::MainFunc, 0);
//     m_state = INIT;
// }

// void Fiber::threadCall() {
//     SetThis(this);
//     m_state = EXEC;
//     if(swapcontext(&t_threadFiber->m_ctx, &m_ctx)) {
//         RLEAVRS_ASSERT_W(false, "swapcontext");
//     }
// }

// void Fiber::threadBack() {
//     SetThis(t_threadFiber.get());
//     if(swapcontext(&m_ctx, &t_threadFiber->m_ctx)) {
//         RLEAVRS_ASSERT_W(false, "swapcontext");
//     }
// }

// //切换到当前协程执行
// void Fiber::fiberSwapIn() {
//     SetThis(this);
//     RLEAVRS_ASSERT(m_state != EXEC);
//     m_state = EXEC;
//     if(swapcontext(&Scheduler::GetMainFiber()->m_ctx, &m_ctx)) {
//         RLEAVRS_ASSERT_W(false, "swapcontext");
//     }
// }

// //切换到后台执行
// void Fiber::fiberSwapOut() {
//     SetThis(Scheduler::GetMainFiber());
//     if(swapcontext(&m_ctx, &Scheduler::GetMainFiber()->m_ctx)) {
//         RLEAVRS_ASSERT_W(false, "swapcontext");
//     }
// }

// //设置当前协程
// void Fiber::SetThis(Fiber* f) {
//     t_fiber = f;
// }

// //返回当前协程
// Fiber::ptr Fiber::GetThis() {
//     if(t_fiber) {
//         return t_fiber->shared_from_this();
//     }
//     Fiber::ptr main_fiber(new Fiber);
//     RLEAVRS_ASSERT(t_fiber == main_fiber.get());
//     t_threadFiber = main_fiber;
//     return t_fiber->shared_from_this();
// }

// //协程切换到后台，并且设置为Ready状态
// void Fiber::YieldToReady() {
//     Fiber::ptr cur = GetThis();
//     RLEAVRS_ASSERT(cur->m_state == EXEC);
//     cur->m_state = READY;
//     cur->fiberSwapOut();
// }

// //协程切换到后台，并且设置为Hold状态
// void Fiber::YieldToHold() {
//     Fiber::ptr cur = GetThis();
//     RLEAVRS_ASSERT(cur->m_state == EXEC);
//     //cur->m_state = HOLD;
//     cur->fiberSwapOut();
// }

// //总协程数
// uint64_t Fiber::TotalFibers() {
//     return s_fiber_count;
// }

// void Fiber::MainFunc() {
//     Fiber::ptr cur = GetThis();
//     RLEAVRS_ASSERT(cur);
//     try {
//         cur->m_cb();
//         cur->m_cb = nullptr;
//         cur->m_state = TERM;
//     } catch (std::exception& ex) {
//         cur->m_state = EXCEPT;
//         RLEAVRS_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
//             << " fiber_id=" << cur->getId()
//             << std::endl
//             << rleavrs::BacktraceToString();
//     } catch (...) {
//         cur->m_state = EXCEPT;
//         RLEAVRS_LOG_ERROR(g_logger) << "Fiber Except"
//             << " fiber_id=" << cur->getId()
//             << std::endl
//             << rleavrs::BacktraceToString();
//     }

//     auto raw_ptr = cur.get();
//     cur.reset();
//     raw_ptr->fiberSwapOut();

//     RLEAVRS_ASSERT_W(false, "never reach fiber_id=" + std::to_string(raw_ptr->getId()));
// }

// void Fiber::CallerMainFunc() {
//     Fiber::ptr cur = GetThis();
//     RLEAVRS_ASSERT(cur);
//     try {
//         cur->m_cb();
//         cur->m_cb = nullptr;
//         cur->m_state = TERM;
//     } catch (std::exception& ex) {
//         cur->m_state = EXCEPT;
//         RLEAVRS_LOG_ERROR(g_logger) << "Fiber Except: " << ex.what()
//             << " fiber_id=" << cur->getId()
//             << std::endl
//             << rleavrs::BacktraceToString();
//     } catch (...) {
//         cur->m_state = EXCEPT;
//         RLEAVRS_LOG_ERROR(g_logger) << "Fiber Except"
//             << " fiber_id=" << cur->getId()
//             << std::endl
//             << rleavrs::BacktraceToString();
//     }

//     auto raw_ptr = cur.get();
//     cur.reset();
//     raw_ptr->threadBack();
//     RLEAVRS_ASSERT_W(false, "never reach fiber_id=" + std::to_string(raw_ptr->getId()));

// }

// }
