#include "mu_test.h"
#include <vector>
#include "tcpclient.hpp"
#include "socket.hpp"
#include "imsg_handler.hpp"
#include "ievent.hpp"
#include "topic.hpp"
#include "json.hpp"
using json = nlohmann::json;


using namespace advcpp;

class TestHandler : public IHandler{
public:
    TestHandler(std::vector<std::string> & a_vec):m_vec(a_vec){}
    void Handle(std::string a_msg , ISocket* a_socket , IDecoder &a_decoder) override
    {
        std::cout << a_msg << std::endl;
        m_vec.push_back(a_msg);        
    }
    std::vector<std::string> &m_vec;
};


class SmokeEvent : public IEvent
{
public:
    SmokeEvent(EventTopic a_topic);

};


BEGIN_TEST(msg)
{
    EventDecoder decoder;
    std::vector<std::string> vec;
    TestHandler testHandler(vec);
    TcpClient clientServer("127.0.0.1" , 2008 , testHandler , decoder);
    std::string msg = "Message from server";
    clientServer.Send("Hello from client");
    clientServer.Receive();
    clientServer.Send("Hello from client");
    clientServer.Receive();
    clientServer.Shutdown();

    ASSERT_EQUAL(vec.size() , 2);
    ASSERT_EQUAL(vec[0] , msg);
}

END_TEST

BEGIN_TEST(send_json)
    EventDecoder decoder;
    std::vector<std::string> vec;
    TestHandler testHandler(vec);
    TcpClient clientServer("127.0.0.1" , 2008 , testHandler , decoder);
    json j;
    j["Type"] = "Smoke";
    j["Floor"] = 2;
    j["Room"] = 205;
    j["Data"] = "None";
    clientServer.Send(j.dump().c_str());
    clientServer.Receive();
    clientServer.Shutdown();

    ASSERT_PASS();

END_TEST


BEGIN_SUITE(Client)
    IGNORE_TEST(msg)
    TEST(send_json)

END_SUITE
