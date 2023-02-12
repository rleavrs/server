#include "tcp_server.h"
#include "log.h"

namespace rleavrs {

static Logger::ptr g_logger = RLEAVRS_LOG_NAME("system");

TcpServer::TcpServer(IOManager* woker,IOManager* io_woker,IOManager* accept_worker)
    :m_worker(woker), 
    m_ioworker(io_woker),
    m_acceptWorker(accept_worker),
    m_recvTimeout((uint64_t)(60 * 1000 * 2)),
    m_isStop(true)  {}
                
TcpServer::~TcpServer() {
    for(auto& i : m_socks) {
        i->close();
    }
    m_socks.clear();
}

bool TcpServer::start() {
    if(!m_isStop) return true;
    m_isStop = false;
    
    for(auto& sock : m_socks) {
        m_acceptWorker->schedule(std::bind(&TcpServer::startAccept,
                                shared_from_this(),
                                sock));
    }
    return true;
}


void TcpServer::stop() {
    m_isStop = true;
    auto self = shared_from_this();
    m_acceptWorker->schedule([this,self]() {
        for(auto& sock : m_socks) {
            sock->cancelAccept();
            sock->close();
        }
        m_socks.clear();
    });
}

bool TcpServer::bind(Address::ptr addr,bool ssl) {
    std::vector<Address::ptr> addrs;
    std::vector<Address::ptr> fails;
    addrs.push_back(addr);
    return bind(addrs, fails, ssl);
}

bool TcpServer::bind(const std::vector<Address::ptr>& addrs,
                    std::vector<Address::ptr>& fails,
                    bool ssl) {
    m_ssl = ssl;
    for(auto& addr : addrs) {
        Socket::ptr sock = ssl ? SSLSocket::CreateTCP(addr) : Socket::CreateTCP(addr);
        if(!sock->bind(addr)) {
            fails.push_back(addr);
            continue;
        }
        
        if(!sock->listen()) {
            fails.push_back(addr);
            continue;
        }
        m_socks.push_back(sock);
    }    
    
    if(!fails.empty()) {
        m_socks.clear();
        return false;
    }

    for(auto& i : m_socks) {
        
    }
    return true;
}

bool TcpServer::loadCertificates(const std::string& cert_file,
                        const std::string& key_file) {
    for(auto& i : m_socks) {
        auto ssl_socket  = std::dynamic_pointer_cast<SSLSocket>(i);
        if(ssl_socket) {
            if(!ssl_socket->loadCertificates(cert_file, key_file)) {
                return false;
            }
        }
    }
    return true;

}

void TcpServer::setName(const std::string& v) {
    m_name = v;    
}

void TcpServer::setRecvTimeout(uint64_t v) {
    m_recvTimeout = v;
}

void TcpServer::setConf(TcpServerConf::ptr v) {
    m_conf = v;
}

void TcpServer::setConf(const TcpServerConf& v) {
    m_conf.reset(new TcpServerConf(v)); // copy construct
}

void TcpServer::handleClient(Socket::ptr client) {
    RLEAVRS_LOG_INFO(g_logger) << "handleClient: " << *client;    
}

void TcpServer::startAccept(Socket::ptr sock){
    while(!m_isStop) {
        Socket::ptr client = sock->accept();
        if(client) {
            client->setRecvTimeout(m_recvTimeout);
            m_ioworker->schedule(std::bind(&TcpServer::handleClient,
                                            shared_from_this(),
                                            client));

        }   else {
            RLEAVRS_LOG_ERROR(g_logger) << "accept error";
        }

    }
}


}