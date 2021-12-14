#ifndef ISOCKET_HPP
#define ISOCKET_HPP
#include <iostream>
#include <memory>

namespace advcpp
{
class ISocket{
public:
    ISocket(ISocket const&);
    ISocket(){};
    virtual ~ISocket() = default;

    virtual void Connect() = 0;
    virtual void Bind() = 0;
    virtual void Listen(int a_backLog) = 0;
    virtual void Reuse() = 0;
    virtual ISocket* Accept() = 0;
    virtual void Send(const char* a_msg) = 0;
    virtual size_t Receive(char* a_buffer , size_t a_bufferSize) = 0;
    virtual void Close() = 0;

    virtual int GetSocket() const = 0;

};

}



#endif