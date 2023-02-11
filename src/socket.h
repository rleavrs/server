#ifndef __RLEAVRS_SOCKET_H__
#define __RLEAVRS_SOCKET_H__

#include <sys/socket.h>
#include <sys/types.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <memory>

namespace rleavrs {
    
class Socket : public std::enable_shared_from_this<Socket>{
public:
    typedef std::shared_ptr<Socket> ptr;
    typedef std::weak_ptr<Socket> weak_ptr;
    
    enum Type {};
    enum Family {};

    static Socket::ptr CreateTCP();
    static Socket::ptr CreateUDP();
    static Socket::ptr CreateTCPSocket();
    static Socket::ptr CreateUDPSocket();
    static Socket::ptr CreateTCPSocket6();
    static Socket::ptr CreateUDPSocket6();
    static Socket::ptr CreateUnixTCPSocket();
    static Socket::ptr CreateUnixUDPSocket();

    










}

}

#endif