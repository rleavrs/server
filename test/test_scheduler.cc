#include "../src/scheduler.h"
#include "../src/utility.h"
#include <atomic>

using namespace rleavrs;
using namespace std;


static rleavrs::Logger::ptr g_logger = RLEAVRS_LOG_NAME("system");
static atomic<int> x = 10;

static int k = 0;

void test_fiber() {
    x--;
    RLEAVRS_LOG_WARN(g_logger) << x;
}

int main(int argc, char** argv) {
    RLEAVRS_LOG_DEBUG(g_logger) << "main";
    rleavrs::Scheduler sc(2, true, "test");
    for(int i = 0; i <= 10; i++) {
        sc.schedule(test_fiber);
    }
    RLEAVRS_LOG_DEBUG(g_logger) << "schedule";
    sc.start();
    RLEAVRS_LOG_DEBUG(g_logger) << "schedule OK";
    sc.stop();
    RLEAVRS_LOG_DEBUG(g_logger) << "over";
    return 0;
}
