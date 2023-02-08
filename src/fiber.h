#ifndef __RLEAVRS__FIBER_H__
#define __RLEAVRS__FIBER_H__

#include <stdint.h>
#include <unistd.h>
#include <memory>
#include <ucontext.h>
#include <functional>

namespace rleavrs {

class Sechduler;

class Fiber {
public:
    typedef std::shared_ptr<Fiber> ptr;

    enum State {
        INIT,
        HOLD,
        EXEC,
        TERM,
        READY,
        EXCEPT
    };

private:
    Fiber();

public:
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller = false);
    ~Fiber();
    void reset(std::function<void()> cb);
    void swapIn();
    void swapOut();
    void call();
    void back();
    uint64_t getId() const;
    State getState() const;
    
    static void SetThis(Fiber* f);
    static Fiber::ptr GetThis();
    static void YieldToReady();
    static void YieldToHold();
    static uint64_t TotalFibers();
    static void MainFunc();
    static void CallerMainFunc();
    static uint64_t GetFiberId();

private:
    uint64_t m_id = 0;
    uint32_t m_stacksize = 0;
    State m_state = State::INIT;
    ucontext_t m_ctx;
    void* m_stack = nullptr;
    std::function<void()> m_cb;
};

}

#endif