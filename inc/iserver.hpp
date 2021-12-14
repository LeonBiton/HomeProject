#ifndef ISERVER_HPP
#define ISERVER_HPP

namespace advcpp{

class IServer{
public:
    virtual ~IServer() = default;

    virtual void Run() = 0;
    virtual void Shutdown() = 0;

};
}

#endif
