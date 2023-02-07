#include "../src/log.h"

#include <iostream>
#include <algorithm>
#include <string>


using namespace rleavrs;
using namespace std;

static Logger::ptr g_logger = LOG_ROOT();
static Logger::ptr r_logger = LOG_NAME("123");

enum A {
    a = 0,
    b = 1,
    c = 2
};

int main() {
    LOG_DEBUG(g_logger) << "123123" << endl;
    LOG_ERROR(r_logger) << "debug" << endl;
}