#ifndef IHANDLER_HPP
#define IHANDLER_HPP
#include <string>
#include "isocket.hpp"
#include "idecoder.hpp"

using namespace advcpp;

namespace advcpp
{

class IHandler{
public:
    virtual ~IHandler() = default;

    virtual void Handle(std::string a_msg , ISocket* a_socket , IDecoder &a_decoder) = 0;

};
}

#endif
