#include "fiber.h"
#include <atomic>
#include "utility.h"
#include "scheduler.h"

namespace rleavrs {

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
        
    }

    ++s_fiber_count;
}


Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool use_caller)
    :m_id(++s_fiber_id),
    m_cb(cb) {
    ++s_fiber_count;
    m_stacksize = stacksize ? stacksize : 1024;
    
    m_stack = MallocStackAllocator::Alloc(1024);
    // alloc
    
    if(getcontext(&m_ctx)) {
        RLEAVRS_ASSERT_W (false, "getcontext");
    }
    
    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;
    
}

Fiber::~Fiber() {

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

void Fiber::swapIn() {
    // SetThis(this);
    // RLEAVRS_ASSERT(m_state != EXEC);
    // m_state = EXEC;
    // if(swapcontext(&Sechduler::GetMainFIber))
}
void Fiber::swapOut() {

}
void Fiber::call() {
    SetThis(this);
    m_state = EXEC;
    if(swapcontext(&t_threadFiber->m_ctx,&m_ctx)) {
        RLEAVRS_ASSERT_W(false, "swapcontext");
    }
}
void Fiber::back() {
    SetThis(t_threadFiber.get());
    if(swapcontext(&m_ctx, &t_threadFiber->m_ctx)) {
        RLEAVRS_ASSERT_W(false, "swapcontext");
    }

}
uint64_t Fiber::getId() const {
    return m_id;
}

Fiber::State Fiber::getState() const {
    return m_state;
}

void Fiber::SetThis(Fiber* f) {
    t_fiber = f;
}

Fiber::ptr Fiber::GetThis() {
    return t_threadFiber;
}

uint64_t Fiber::GetFiberId() {
    return s_fiber_id;
}

void Fiber::YieldToReady() {
    Fiber::ptr cur = GetThis();
    RLEAVRS_ASSERT(cur->m_state == EXEC);
    cur -> m_state = READY;
    cur -> swapOut();
}

void Fiber::YieldToHold() {
    Fiber::ptr cur = GetThis();
    RLEAVRS_ASSERT(cur->m_state == EXEC);
    cur -> m_state = HOLD;
    cur -> swapOut();    
}

uint64_t Fiber::TotalFibers() {
    return s_fiber_count;
}

void Fiber::MainFunc() {
    Fiber::ptr cur = GetThis();
    
    cur->m_cb();
    cur->m_cb = nullptr;
    cur->m_state = TERM;
    
    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->swapOut();
}

void Fiber::CallerMainFunc() {
    Fiber::ptr cur = GetThis();
    
    cur->m_cb();
    cur->m_cb = nullptr;
    cur-> m_state = TERM;

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->back();

}



}