#include "iomanager.h"
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>
#include "hook.h"

namespace rleavrs {

static Logger::ptr g_logger = RLEAVRS_LOG_NAME("system");


IOManager::FdContext::EventContext& IOManager::FdContext::getContext(EventStatus event) {
    switch(event) {
        case IOManager::READ:   return read;
        case IOManager::WRITE:  return write;
        default: RLEAVRS_ASSERT_W(false, "getContext");
    }
}

void IOManager::FdContext::resetContext(EventContext& ctx) {
    ctx.scheduler = nullptr;
    ctx.fiber.reset();
    ctx.cb = nullptr;
}

void IOManager::FdContext::triggerEvent(IOManager::EventStatus event) {
    RLEAVRS_ASSERT(events & event);
    events = (EventStatus)(events & ~event);    //TODE: why?
    EventContext& ctx = getContext(event);
    if(ctx.cb) {
        ctx.scheduler->schedule(&ctx.cb);
    } else {
        ctx.scheduler->schedule(&ctx.fiber);
    }
    ctx.scheduler = nullptr;
}

IOManager::IOManager(size_t threads, bool use_caller, const std::string& name)
    :Scheduler(threads, use_caller,name) {  // father contruct
    m_epfd = epoll_create(5000);
    int rt = pipe(m_tickleFds);
    epoll_event event;
    memset(&event, 0, sizeof(epoll_event));
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = m_tickleFds[0];
    rt = fcntl(m_tickleFds[0], F_SETFL, O_NONBLOCK);
    RLEAVRS_ASSERT(!rt);
    
    rt = epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_tickleFds[0], &event);
    RLEAVRS_ASSERT(!rt);
    
    contextResize(32);
    start();
}


IOManager::~IOManager() {}


void IOManager::contextResize(size_t size) {
    m_fdContexts.resize(size);
    for(size_t i = 0; i < m_fdContexts.size(); ++i) {
        if(!m_fdContexts[i]) {
            m_fdContexts[i] = new FdContext();
            m_fdContexts[i]->fd = i;
        }
    }
}

int IOManager::addEvent(int fd, EventStatus event, std::function<void()>  cb) {
    FdContext* fd_ctx = nullptr;
    RWMutexType::ReadLock lock(m_mutex);
    if((int)m_fdContexts.size()  > fd) {
        fd_ctx = m_fdContexts[fd];
        lock.unlock();
    } else {
        lock.unlock();
        RWMutexType::WriteLock lock_w(m_mutex);
        contextResize(fd * 1.5);
        fd_ctx = m_fdContexts[fd];
    }
    
    int op = fd_ctx->events ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
    epoll_event epevent;
    epevent.events = EPOLLET | fd_ctx->events | event;
    epevent.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &epevent);
    if(rt)  return -1;

    ++m_pendingEventCount;
    fd_ctx->events = (EventStatus)(fd_ctx->events | event);
    FdContext::EventContext& event_ctx = fd_ctx->getContext(event);
    event_ctx.scheduler = Scheduler::GetThis();
    if(cb) {
        event_ctx.cb.swap(cb);
    } else {
        event_ctx.fiber = Fiber::GetThis();
    }

    return 0;
}

bool IOManager::delEvent(int fd, EventStatus event) {
    RWMutexType::ReadLock lock(m_mutex);
    if((int)m_fdContexts.size() <= fd) {
        return false;
    }
    FdContext* fd_ctx = m_fdContexts[fd];
    lock.unlock();
    
    FdContext::MutexType::Lock  lock2(fd_ctx->mutex);
    if(!(fd_ctx->events & ~event))  return false;
    EventStatus new_event = (EventStatus)(fd_ctx->events & ~event);
    int op = new_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
    epoll_event epevent;
    epevent.events = EPOLLET | new_event;
    epevent.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &epevent);
    if(rt)  return false;

    --m_pendingEventCount;
    fd_ctx->events = new_event;
    FdContext::EventContext& event_ctx = fd_ctx->getContext(event);
    fd_ctx->resetContext(event_ctx);    
    return true;
}

bool IOManager::cancelEvent(int fd, EventStatus event) {
    RWMutexType::ReadLock lock(m_mutex);
    if(fd >= (int)m_fdContexts.size())    return false;
    FdContext* fd_ctx =  m_fdContexts[fd];
    lock.unlock();
    
    FdContext::MutexType::Lock lock2(fd_ctx->mutex);
    if(!(fd_ctx->events & ~event))  return false;
    EventStatus new_event = (EventStatus)(fd_ctx->events & ~event);
    int op = new_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
    epoll_event epevent;
    epevent.events = EPOLLET | new_event;
    epevent.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &epevent);
    if(rt)  return false;
    
    fd_ctx->triggerEvent(event);        // ?
    --m_pendingEventCount;
    return true;
}

bool IOManager::cancelAll(int fd) {
    RWMutexType::ReadLock lock(m_mutex);
    if((int)m_fdContexts.size() <= fd) {return false;}
    FdContext* fd_ctx = m_fdContexts[fd]; 
    m_mutex.unlock();
    
    FdContext::MutexType::Lock lock2(fd_ctx->mutex);
    if(!fd_ctx->events) { return false; }
    int op = EPOLL_CTL_DEL;
    epoll_event epevent;
    epevent.events = 0;
    epevent.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &epevent);
    if(rt)  return false;

    if(fd_ctx->events & READ) {
        fd_ctx->triggerEvent(READ);
        --m_pendingEventCount;
    }
    
    if(fd_ctx->events & WRITE) {
        fd_ctx->triggerEvent(WRITE);
        --m_pendingEventCount;
    }
    return true;
}

IOManager* IOManager::GetThis() {
    return dynamic_cast<IOManager*>(Scheduler::GetThis());
}

void IOManager::tickle() {
    if(!hasIdleThread()) {
        return;
    }
    int rt = write(m_tickleFds[1], "T",1);
}


bool IOManager::stopping(uint64_t& timeout) {
    timeout = getNextTime();
    return timeout == ~0ull && m_pendingEventCount == 0
                            && Scheduler::stopping();
}

bool IOManager::stopping() {
    uint64_t timeout = 0;
    return stopping(timeout);    
}

void IOManager::idle() {
    const uint64_t MAX_EVENTS = 256;
    epoll_event* events = new epoll_event[MAX_EVENTS]();
    std::shared_ptr<epoll_event> shared_events(events, [](epoll_event* ptr) {
        delete[] ptr;       // ??
    });
    
    // while(true) {
        

        
    // }
}

void IOManager::onTimerInsertedAtFront() {  tickle(); }




}