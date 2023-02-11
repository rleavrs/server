#include "src/log.h"

#include <iostream>
#include <algorithm>
#include <string>


using namespace rleavrs;
using namespace std;

static Logger::ptr g_logger = RLEAVRS_LOG_ROOT();
static Logger::ptr r_logger = RLEAVRS_LOG_NAME("123");

int main() {
    // LogFormatter* x = new LogFormatter("mprctndflTFN");
    while(1){
        RLEAVRS_LOG_DEBUG(g_logger) << "123123" ;
        RLEAVRS_LOG_ERROR(r_logger) << "debug";
        sleep(0.1);
    }

}