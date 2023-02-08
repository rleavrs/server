#include "timer.h"

namespace rleavrs  {


bool Timer::Comparator::operator()(const Timer::ptr& lhs
                        ,const Timer::ptr& rhs) const {
    if(!lhs && !rhs) {
        return false;
    }
    if(!lhs) {
        return true;
    }
    if(!rhs) {
        return false;
    }
    if(lhs->m_next < rhs->m_next) {
        return true;
    }
    if(rhs->m_next < lhs->m_next) {
        return false;
    }
    return lhs.get() < rhs.get();
}

bool Timer::cancel() {
    TimerManager::RWMutexType::WriteLock lock(m_manager->m_mutex);
    if(m_cb) {
        m_cb = nullptr;
        auto it = m_manager->m_timers.find(shared_from_this());
        m_manager->m_timers.erase(it);
        return true;
    }
    return true;
}

bool Timer::refresh() {
    TimerManager::RWMutexType::WriteLock lock(m_manager->m_mutex);
    if(!m_cb) {
        return false;        
    }
    auto it = m_manager->m_timers.find(shared_from_this());
    if(it == m_manager->m_timers.end()) {
        return false;
    }
    
    m_manager->m_timers.erase(it);
    m_next = rleavrs::GetCurrentMS() + m_ms;
    m_manager->m_timers.insert(shared_from_this());
    return true;
}

bool Timer::reset(uint64_t ms, bool from_now) {
    if(ms == m_ms && !from_now) return true;
    TimerManager::RWMutexType::WriteLock lock(m_manager->m_mutex);
    if(!m_cb)   return false;
    auto it = m_manager->m_timers.find(shared_from_this());
    if(it == m_manager->m_timers.end()) return false;
    m_manager->m_timers.erase(it);
    uint64_t start = 0;
    if(from_now) {
        start = rleavrs::GetCurrentMS();
    } else {
        start = m_next - m_ms;
    }
    m_ms = ms;
    m_next = m_ms + start;
    m_manager->addTimer(shared_from_this(),lock);
    return true;
}

Timer::Timer(uint64_t ms, std::function<void()> cb,
        bool recurring, TimerManager* manager)
        :m_ms(ms),
        m_cb(cb),
        m_recurring(recurring),
        m_manager(manager) {
            m_next = rleavrs::GetCurrentMS() + m_ms;
        }

Timer::Timer(uint64_t next):
        m_next(next) {}









}