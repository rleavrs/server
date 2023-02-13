#include "../src/socket.h"
#include "../src/log.h"
#include "../src/iomanager.h"

using namespace rleavrs;
static Logger::ptr g_logger = RLEAVRS_LOG_NAME("server");

void run() {
    IPAddress::ptr addr = Address::LookupAnyIPAddress("0.0.0.0:2345");
    Socket::ptr sock = Socket::CreateUDP(addr);

    if(sock->bind(addr)) {
        RLEAVRS_LOG_INFO(g_logger) << "udp bind : " << *addr;
    } else {
        RLEAVRS_LOG_ERROR(g_logger) << "udp bind : " << *addr;
        return;
    }

    while(true) {
        char buff[1024];
        Address::ptr from(new IPv4Address);
        int len = sock->recvFrom(buff, 1024, from);
        if(len > 0) {
            buff[len] = '\0';
            RLEAVRS_LOG_INFO(g_logger) << "recv: " << buff << " from: " << *from;
            len = sock->sendTo(buff, len, from);
            if(len < 0) {
            RLEAVRS_LOG_ERROR(g_logger) << "send: " << buff << " to: " << *from
                << " error=" << len;
            }
        }
    }
}

int main() {
    IOManager iom(1);
    iom.schedule(run);
    return 0;
}
