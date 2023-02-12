#include "src/log.h"

#include <iostream>
#include <algorithm>
#include <string>


using namespace rleavrs;
using namespace std;

static Logger::ptr g_logger = RLEAVRS_LOG_NAME("system");
int main() {
    // LogFormatter* x = new LogFormatter("mprctndflTFN");
    while(1){
        RLEAVRS_LOG_INFO(g_logger) << "INFO" ;
        RLEAVRS_LOG_DEBUG(g_logger) << "DEBUG" ;
        RLEAVRS_LOG_ERROR(g_logger) << "ERROR" ;
        RLEAVRS_LOG_FATAL(g_logger) << "FATAL" ;
        sleep(0.1);
    }

}