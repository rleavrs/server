#include "../src/log.h"

#include <iostream>
#include <algorithm>
#include <string>

using namespace rleavrs;
using namespace std;

int main() {

    std::string x = "123";
    LogEvent::ptr e = std::make_shared<LogEvent>();
    
    Logger L;
    L.registEvent(e);
    L.log();
}