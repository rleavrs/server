#include "../src/socket.h"
#include "../src/iomanager.h"
#include "../src/log.h"
#include <stdlib.h>

using namespace rleavrs;

static Logger::ptr g_logger = RLEAVRS_LOG_NAME("client");

const char* ip = nullptr;
uint16_t port = 0;

void run() {
    auto addr = Address::LookupAnyIPAddress(ip);
    if(!addr) {
        RLEAVRS_LOG_ERROR(g_logger) << "invalid ip : " << ip;
        return;
    } 
    addr->setPort(port);
    auto sock = Socket::CreateUDP(addr);
    RLEAVRS_LOG_INFO(g_logger) << "sock connecting";
    IOManager::GetThis()->schedule([sock]() {
        Address::ptr addr(new IPv4Address);
        while(true) {
            char buff[1024];
            int len = sock->recvFrom(buff, 1024, addr);
            if(len > 0) {
                RLEAVRS_LOG_INFO(g_logger) << "recv : " << std::string(buff,len) 
                                            << "from : " << *addr << std::endl; 
            }
        }
    });
    
    RLEAVRS_LOG_INFO(g_logger) << "client start work";

    // sleep(1);
    while(true) {
        std::string line;
        std::cout << "input>";
        std::getline(std::cin, line);
        if(!line.empty()) {
            int len = sock->sendTo(line.c_str(), line.size(), addr);
            if(len < 0) {
                int err = sock->getError();
                RLEAVRS_LOG_ERROR(g_logger) << "send error err=" << err
                        << " errstr=" << strerror(err) << " len=" << len
                        << " addr=" << *addr
                        << " sock=" << *sock;
            } else {
                RLEAVRS_LOG_INFO(g_logger) << "send " << line << " len:" << len;
            }
        }
    }
}

int main (int argc, char** argv){
    if(argc < 3) {
        RLEAVRS_LOG_FATAL(g_logger) << "use as [" << argv[0] << " ip port]";
        return 0;
    }

    ip = argv[1];
    port =  atoi(argv[2]);

    IOManager iom(2);
    iom.schedule(run);
    return 0;
}