#ifndef __RLEAVRS__FIBER_H__
#define __RLEAVRS__FIBER_H__

#include <stdint.h>
#include <unistd.h>
#include <memory>
#include <ucontext.h>
#include <functional>


// Fiber 
// makecontext:初始化一个ucontext_t,func参数指明了该context的入口函数，argc为入口参数的个数，每个参数的类型必须是int类型。
//              另外在makecontext之前，一般需要显示的初始化栈信息以及信号掩码集同时也需要初始化uc_link，以便程序退出上下文后继续执行。
// swapcontext:原子操作，该函数的工作是保存当前上下文并将上下文切换到新的上下文运行。
// getcontext:将当前的执行上下文保存在ucp中，以便后续恢复上下文
// setcontext : 将当前程序切换到新的context,在执行正确的情况下该函数直接切换到新的执行状态，不会返回。

namespace rleavrs {

class Scheduler;

class Fiber : public std::enable_shared_from_this<Fiber>{
public:
    friend Scheduler;
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
    void fiberSwapIn();
    void fiberSwapOut();
    void threadCall();
    void threadBack();
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