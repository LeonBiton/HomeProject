#include "iagent.hpp"
#include "isubscriber.hpp"
extern "C"
IAgent * MakeAgent(size_t a_id , std::string a_ip , int a_port , IHandler &a_handler,
 ISubscribers &a_subscriber , IDecoder & a_decoder);
