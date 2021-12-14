#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <string>
#include <exception>
#include <sys/types.h> // for socket 
#include <sys/socket.h>// for socket 
#include <netinet/in.h>// for socket 
#include <unistd.h> //for close
#include <string.h>    //for memset
#include <arpa/inet.h>
#include <memory>
#include "isocket.hpp"

using namespace advcpp;

namespace advcpp{

class Socket : public ISocket{
public:
    Socket(std::string a_ip , int a_port , int a_domain = AF_INET, int a_type = SOCK_STREAM);
    Socket() = default;
    explicit Socket(int a_socket);
    Socket& operator=(Socket const& a_other) = default;
    ~Socket();

    void Connect() override;
    void Bind() override;
    void Listen(int a_backLog) override;
    void Reuse() override;
    ISocket* Accept() override;
    void Send(const char* a_msg) override;
    size_t Receive(char* a_buffer , size_t a_bufferSize) override;
    void Close() override;

    int GetSocket() const override;

private:
    int m_socket;
    std::string m_ip;
    int m_port;
};
}

#endif
