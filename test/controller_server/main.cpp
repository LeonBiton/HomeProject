#include "mu_test.h"
#include "tcpserver.hpp"
#include "socket.hpp"
#include "imsg_handler.hpp"
#include "notifyer.hpp"
#include "subscribers_manager.hpp"
#include "event_decoder.hpp"
#include "tcpclient.hpp"
#include "event_decoder.hpp"
#include "event_task.hpp"

class ControllerHandler : public IHandler
{
    void Handle(std::string a_msg , ISocket* a_socket , IDecoder &a_decoder) override
    {
        std::cout << a_msg << std::endl;
    }

};


BEGIN_TEST(server)
    EventDecoder decoder;
    ControllerHandler handler;
    TcpServer server("127.0.0.1" , 2010 , handler ,decoder);
    server.Run();
    ASSERT_PASS();

END_TEST


BEGIN_SUITE(Server)
    TEST(server)

END_SUITE