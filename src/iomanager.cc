#include "iomanager.h"
#include <fcntl.h>
#include <sys/epoll.h>

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
    :Scheduler(threads, use_caller,name) {
    
    
}

IOManager::~TimerManager() {
    stop();
}

















}