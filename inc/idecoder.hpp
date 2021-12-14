#ifndef IDECODER_HPP
#define IDECODER_HPP 
#include "ievent.hpp"

using namespace advcpp;

namespace advcpp
{

class IDecoder{
public:
    virtual ~IDecoder() = default;

    virtual IEvent * Decode(std::string a_msg) = 0;

};
}

#endif