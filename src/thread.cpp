#include "thread.h"

namespace rleavrs {


Thread::Thread(std::function<void()> cb, const std::string name) {}
Thread::~Thread() {}

const std::string& Thread::getName() const  {return m_name;}
const tid_t getId() const {return m_id;}
void Thread::join(){} 
static Thread* Thread::GetThis() {
       
}
static const std::string& Thread::GetName();







}