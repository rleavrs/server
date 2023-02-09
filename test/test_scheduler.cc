#include "../src/scheduler.h"

using namespace rleavrs;
using namespace std;


static rleavrs::Logger::ptr g_logger = RLEAVRS_LOG_ROOT();

void test_fiber() {
    static int s_count = 1;
    RLEAVRS_LOG_DEBUG(g_logger) << "test in fiber s_count=" << s_count << endl;

    // sleep(0.);
    if(++s_count) {
        rleavrs::Scheduler::GetThis()->schedule(&test_fiber, rleavrs::GetThreadId());
    }
}

int main(int argc, char** argv) {
    RLEAVRS_LOG_DEBUG(g_logger) << "main" << endl;
    rleavrs::Scheduler sc(3, false, "test");
    sc.start();
    RLEAVRS_LOG_DEBUG(g_logger) << "schedule" << endl;
    sc.schedule(&test_fiber);
    RLEAVRS_LOG_DEBUG(g_logger) << "schedule OK" << endl;
    
    sc.stop();
    RLEAVRS_LOG_INFO(g_logger) << "over";
    // return 0;
}
