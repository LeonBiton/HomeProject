#include "tcpclient.hpp"

using namespace advcpp;

TcpClient::TcpClient(std::string a_ip , int a_port , IHandler &a_handler , IDecoder &a_decoder)
:m_socket(new Socket(a_ip , a_port))
,m_handler(a_handler)
,m_decoder(a_decoder)
{
    m_socket->Connect();
}

void TcpClient::Send(const std::string &a_msg)
{
    m_socket->Send(a_msg.c_str());
}

size_t TcpClient::Receive()
{
    char buffer[BUFFER_SIZE];
    size_t readBytes = m_socket->Receive(buffer , BUFFER_SIZE);
    std::string msg(buffer);
    m_handler.Handle(msg , m_socket , m_decoder);
    return readBytes;
}

void TcpClient::Shutdown()
{
    m_socket->Close();
}