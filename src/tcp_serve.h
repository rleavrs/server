#ifndef __RLEAVRS_TCP_SERVE_H_
#define __RLEAVRS_TCP_SERVE_H_
#include <memory>
#include <vector>
#include 


namespace rleavrs {

struct TcpServeConf {

};

class TcpServe {
public:
    typedef std::shared_ptr<TcpServe> ptr;

    TcpServe();
    virtual ~TcpServe();
    virtual bool bind();
    virtual bool bind();
    bool loadCertificates();
    virtual bool start();
    virtual void stop();


protected:


private:
    std::vector<



};





}





#endif