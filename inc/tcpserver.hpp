#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP
#include <sys/select.h>/*for FD_SET*/
#include <sys/time.h>  /*for FD_SET*/
#include <errno.h>
#include <list>
#include <memory>
#include "iserver.hpp"
#include "imsg_handler.hpp"
#include "socket.hpp"
#include "event_decoder.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace advcpp;

namespace advcpp
{
class TcpServer : public IServer{
public:
    TcpServer(std::string a_ip ,int a_port , IHandler &a_handler ,IDecoder &a_decoder);

    void Run() override;
    void Shutdown() override;

    static const int BACK_LOG = 32;
    static const int BUFFER_SIZE = 4000;

private:
    void CheckNewClients();
    void CheckCurClients(int a_activity); 

private:
    ISocket * m_listeningSocket;
    fd_set m_master;
    fd_set m_temp;
    std::list<ISocket *> m_socketList;
    IHandler &m_handler;
    IDecoder &m_decoder;
};

}


#endif
