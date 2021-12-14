#include "socket.hpp"

using namespace advcpp;

Socket::Socket(std::string a_ip , int a_port , int a_domain, int a_type)
:m_socket(socket(a_domain,a_type,0))
,m_ip(a_ip)
,m_port(a_port)
{
    if(m_socket < 0)
    {
        throw std::runtime_error("socket failed");
    }
}

Socket::Socket(int a_socket)
:m_socket(a_socket){}

Socket::~Socket()
{
    close(m_socket);
}

void Socket::Connect()
{
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(m_ip.c_str());
    sin.sin_port = htons(m_port);
    if (connect(m_socket, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        throw std::runtime_error("connection failed");
    } 
}

void Socket::Bind()
{
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(m_port);
    if (bind(m_socket, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        throw std::runtime_error("bind failed");
    }
}

void Socket::Listen(int a_backLog)
{
    if(listen(m_socket , a_backLog) < 0)
    {
        throw std::runtime_error("listen failed");
    }
}

void Socket::Reuse()
{
    int optval = 1;
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        throw std::runtime_error("reuse failed");
    }
}

ISocket* Socket::Accept()
{
    struct sockaddr_in client_sin;
    unsigned int addr_len = sizeof(client_sin);
    int acceptSocket = accept(m_socket, (struct sockaddr *)&client_sin, &addr_len);
    if(acceptSocket < 0)
    {
        throw std::runtime_error("accept fail");
    }
    ISocket * newSocket = new Socket(acceptSocket);
    return newSocket;
}

void Socket::Send(const char* a_msg)
{
    int dataLength = strlen(a_msg);
    int sendBytes = send(m_socket,a_msg,dataLength,0);
    if (sendBytes < 0)
    {
        throw std::runtime_error("send failed");
    }
}

size_t Socket::Receive(char* a_buffer , size_t a_bufferSize)
{
    int readBytes = recv(m_socket,a_buffer,a_bufferSize,0);
    if (readBytes < 0)
    {
        throw std::runtime_error("recv failed");
    }
    return readBytes;
}

void Socket::Close()
{
    close(m_socket);
}

int Socket::GetSocket() const
{
    return m_socket;
}
