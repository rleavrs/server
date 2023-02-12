#ifndef __RLEAVRS_TCP_SERVE_H_
#define __RLEAVRS_TCP_SERVE_H_
#include <memory>
#include <vector>
#include "socket.h"
#include "iomanager.h"


namespace rleavrs {

struct TcpServerConf {
    typedef std::shared_ptr<TcpServerConf> ptr;
};

class TcpServer : public std::enable_shared_from_this<TcpServer>{
public:
    typedef std::shared_ptr<TcpServer> ptr;

    TcpServer(IOManager* woker = IOManager::GetThis(),
                IOManager* io_woker = IOManager::GetThis(),
                IOManager* accept_worker = IOManager::GetThis() );    
    virtual ~TcpServer();
    virtual bool start();
    virtual void stop();
    virtual bool bind(Address::ptr addr,bool ssl = false);
    virtual bool bind(const std::vector<Address::ptr>& addrs,
                        std::vector<Address::ptr>& fails,
                        bool ssl = false);
    bool loadCertificates(const std::string& cert_file,
                            const std::string& key_file);

    void setName(const std::string& v);
    void setRecvTimeout(uint64_t v);
    void setConf(TcpServerConf::ptr v);
    void setConf(const TcpServerConf& v);

    uint64_t getRecvTimeout() const { return m_recvTimeout; }
    std::string getName() const { return m_name; }
    bool isStop() const { return m_isStop; }
    TcpServerConf::ptr getConf() const { return m_conf; } 
    std::vector<Socket::ptr> getSocks() const {return m_socks; }

protected:
    virtual void handleClient(Socket::ptr client);
    virtual void startAccept(Socket::ptr sock);

private:
    std::vector<Socket::ptr> m_socks;
    IOManager* m_worker;
    IOManager* m_ioworker;
    IOManager* m_acceptWorker;
    uint64_t m_recvTimeout;
    std::string m_name;
    std::string m_type = "tcp";
    bool m_isStop;
    bool m_ssl = false;
    TcpServerConf::ptr m_conf;
};

}


#endif