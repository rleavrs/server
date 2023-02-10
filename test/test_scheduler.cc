#include "../src/scheduler.h"

using namespace rleavrs;
using namespace std;


static rleavrs::Logger::ptr g_logger = RLEAVRS_LOG_ROOT();

static int s_count = 10;

void test_fiber() {
    RLEAVRS_LOG_DEBUG(g_logger) << " " << GetThreadId() <<  " test in fiber s_count=" << s_count << endl;
}

int main(int argc, char** argv) {
    RLEAVRS_LOG_DEBUG(g_logger) << "main" << endl;
    rleavrs::Scheduler sc(6, false, "test");
    sc.start();
    RLEAVRS_LOG_DEBUG(g_logger) << "schedule" << endl;
    while(s_count++){
        sc.schedule(&test_fiber);
        sleep(0.0001);
    }
    RLEAVRS_LOG_DEBUG(g_logger) << "schedule OK" << endl;
    
    sc.stop();
    RLEAVRS_LOG_INFO(g_logger) << "over";
    // return 0;
}
