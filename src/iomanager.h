#ifndef __RLEAVRS_IOMANAGER_H__
#define __RLEAVRS_IOMANAGER_H__
#include "timer.h"
#include "scheduler.h"

namespace rleavrs {

class IOManager : public TimerManager, public Scheduler{
public:
    typedef RWMutex RWMutexType;
    enum EventStatus {
        NONE = 0x0,
        READ = 0x1,
        WRITE = 0x4,
    };

protected:
    IOManager(size_t threads = 1, bool user_caller = true, const std::string& name = "");
    ~IOManager();
    
    int addEvent(int fd, EventStatus event, std::function<void()> cb = nullptr);
    bool delEvent(int fd, EventStatus event);
    bool cancelEvent(int fd, EventStatus event);
    bool cancelAll(int fd);
    static IOManager* GetThis();

protected:
    void tickle() override;
    bool stopping() override;
    void idle() override;
    void onTimerInsertedAtFront() override;

    void contextResize(size_t size);
    bool stopping(uint64_t& timeout);

private:
    class FdContext {
    public:
        typedef Mutex MutexType;
        
        struct EventContext {
            Scheduler* scheduler = nullptr;
            Fiber::ptr fiber;
            std::function<void()> cb;
        };
        
        EventContext& getContext(EventStatus event);
        void resetContext(EventContext& ctx);
        void triggerEvent(EventStatus event);
        
        int fd = 0;
        EventContext read;
        EventContext write;
        EventStatus events = NONE;
        MutexType mutex;
    };

private:
    int m_epfd = 0;
    int m_tickleFds[2];
    std::atomic<size_t> m_pendingEventCount = {0};
    RWMutexType m_mutex;
    std::vector<FdContext*> m_fdContexts;
};





}


#endif