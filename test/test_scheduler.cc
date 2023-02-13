// #include "../src/scheduler.h"
// #include "../src/utility.h"
// #include <atomic>

// using namespace rleavrs;
// using namespace std;


// static rleavrs::Logger::ptr g_logger = RLEAVRS_LOG_NAME("system");
// static atomic<int> x = 10;

// static int k = 0;

// void test_fiber() {
//     while(1){
//         RLEAVRS_LOG_WARN(g_logger) << x;
//         sleep(0.000001);
//     }
// }

// int main(int argc, char** argv) {
//     rleavrs::Scheduler sc(1, true, "test");
//     sc.start();
//     sc.schedule(test_fiber);
//     sc.stop();
//     return 0;
// }

#include "../src/scheduler.h"

static rleavrs::Logger::ptr g_logger = RLEAVRS_LOG_ROOT();

void test_fiber() {
    static int s_count = 1115;
    RLEAVRS_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;

    //sleep(1);
    if(--s_count >= 0) {
        rleavrs::Scheduler::GetThis()->schedule(&test_fiber, rleavrs::GetThreadId());
    }
}

int main(int argc, char** argv) {
    RLEAVRS_LOG_INFO(g_logger) << "main";
    rleavrs::Scheduler sc(3);
    sc.start();

    RLEAVRS_LOG_INFO(g_logger) << "schedule";
    sc.schedule(&test_fiber);
    
    sc.stop();
    RLEAVRS_LOG_INFO(g_logger) << "over";
    return 0;
}
