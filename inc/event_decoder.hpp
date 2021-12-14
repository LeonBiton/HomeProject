#ifndef EVENT_DECODER_HPP
#define EVENT_DECODER_HPP
#include <string>
#include <memory>
#include "genreal_event.hpp"
#include "idecoder.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace advcpp;

namespace advcpp
{
class EventDecoder : public IDecoder
{
public:

    IEvent* Decode(std::string a_buffer);

private:


};
}

#endif