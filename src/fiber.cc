#include "fiber.h"
#include <atomic>

namespace rleavrs {

static std::atomic<uint64_t> s_fiber_id {0};
static std::atomic<uint64_t> s_fiber_count {0};

static thread_local Fiber* t_fiber = nullptr;
static thread_local Fiber::ptr t_threadFiber = nullptr;


Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool use_caller) {

}

Fiber::~Fiber() {

}

void Fiber::reset() {

}
void Fiber::swapIn() {
    
}
void Fiber::swapOut() {

}
void Fiber::call() {

}
void Fiber::back() {

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

}
void Fiber::YieldToHold() {

}
uint64_t Fiber::TotalFibers() {

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
    // cur->m_stack = TERM;

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->back();

}



}