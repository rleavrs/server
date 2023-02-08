#ifndef __RLEAVRS_TIMER_H__
#define __RLEAVRS_TIMER_H__

#include "thread.h"
#include "utility.h"
#include <set>

namespace rleavrs{

class TimerManager;

class Timer : public std::enable_shared_from_this<Timer> {
public:
    friend TimerManager;
    typedef std::shared_ptr<Timer> ptr;

    bool cancel();
    bool refresh();
    bool reset(uint64_t ms, bool from_now);

private:
    Timer(uint64_t ms, std::function<void()> cb,
            bool recurring, TimerManager* manager);
    Timer(uint64_t next);

private:
    class Comparator {
    public:
        bool operator() (const Timer::ptr& lhs, const Timer::ptr& rhs) const;
    };    

private:
    bool m_recurring = false;
    uint64_t m_ms = 0;      // during
    uint64_t m_next = 0;    // o'clock  start_clock = m_next - m_ms
    std::function<void()> m_cb;
    TimerManager* m_manager = nullptr;
};

class TimerManager {
public:
    friend Timer;
    typedef RWMutex RWMutexType;

    TimerManager();
    virtual ~TimerManager() = 0;
    Timer::ptr addTimer(uint64_t ms, std::function<void()> cb, bool recurring = false);
    Timer::ptr addConditionTimer(uint64_t ms, std::function<void()> cb,
                                    std::weak_ptr<void> wead_cond,
                                    bool recurring = false);
    uint64_t getNextTime();
    
    void listExpiredCb(std::vector<std::function<void()>> &cbs);
    
    bool hasTimer();
    
protected:
    
    virtual void onTimerInsertedAteFront() = 0;
    
    void addTimer(Timer::ptr val, RWMutexType::WriteLock& Lock);

private:
    bool detectClockRollover(uint64_t now_ms);
    
private:
    RWMutexType m_mutex;
    std::set<Timer::ptr, Timer::Comparator> m_timers;
    bool m_tickled = false;
    uint64_t m_previouseTime = 0;
};

}
#endif