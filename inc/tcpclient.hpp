#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP 
#include "socket.hpp"
#include "imsg_handler.hpp"
#include "event_decoder.hpp"

using namespace advcpp;

namespace advcpp
{

class TcpClient{
public:
    static const int BUFFER_SIZE = 4000;
    TcpClient(std::string a_ip , int a_port , IHandler &a_handler , IDecoder &a_decoder);

    void Send(const std::string &a_msg);
    size_t Receive();

    void Shutdown();

private:
    ISocket* m_socket;
    IHandler &m_handler;
    IDecoder &m_decoder;

};
}


#endif