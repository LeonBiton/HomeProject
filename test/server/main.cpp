#include "mu_test.h"
#include <dlfcn.h>
#include "tcpserver.hpp"
#include "socket.hpp"
#include "imsg_handler.hpp"
#include "notifyer.hpp"
#include "subscribers_manager.hpp"
#include "event_decoder.hpp"
#include "tcpclient.hpp"
#include "event_router.hpp"
#include "smoke_agent.hpp"


std::shared_ptr<IAgent> CreateAgent(const char* soname ,size_t a_id , std::string a_ip , int a_port , IHandler &a_handler ,
                    ISubscribers &a_subscriber , IDecoder & a_decoder)
{
    void* handle = ::dlopen(soname, RTLD_LAZY);
    if(!handle)
    {
        std::cout << "error loading so: " << ::dlerror() << '\n';
        return nullptr;
    }

    typedef std::shared_ptr<IAgent> (*SmokeAgentFunc)(size_t a_id , std::string a_ip , int a_port , IHandler &a_handler ,
    ISubscribers &a_subscriber , IDecoder & a_decoder);
    SmokeAgentFunc agentFunc = reinterpret_cast<SmokeAgentFunc>( ::dlsym(handle, "MakeAgent") );
    if(!agentFunc)
    {
        std::cout << "error getting function from so: " << ::dlerror() << '\n';
        return nullptr;
    }
    std::shared_ptr<IAgent> agent = agentFunc(a_id , a_ip , a_port , a_handler , a_subscriber , a_decoder);
    return agent;
}


class SmokeEvent : public IEvent{
public:
    SmokeEvent(int a_floor , int a_room):IEvent(EventTopic("Smoke" , a_floor , a_room)){}

};

class DoorEvent : public IEvent{
public:
    DoorEvent(int a_floor , int a_room):IEvent(EventTopic("Door_Open" , a_floor , a_room)){}

};

class MovementAgent : public IAgent , public std::enable_shared_from_this<MovementAgent>{
public:
    MovementAgent(size_t a_id , ISubscribers &a_subscriber):IAgent(a_id) , m_subscriber(a_subscriber){}

    void Notify(std::shared_ptr<IEvent> a_event) override
    {
        std::cout << "got event - " << a_event->GetEventType() << std::endl;
        m_eventReceived = a_event->GetEventTopic();
    }

    void Subscribe(EventTopic a_eventTopic) override
    {
        m_subscriber.Subscribe(a_eventTopic , shared_from_this());
    }

    void Unsubscribe(EventTopic a_eventTopic) override
    {
        m_subscriber.Unsubscribe(a_eventTopic , shared_from_this());
    }

    EventTopic GetEventTopic() const    
    {
        return m_eventReceived;
    }

private:
    EventTopic m_eventReceived;
    ISubscribers &m_subscriber;

};


class ControllerHandler : public IHandler
{
    void Handle(std::string a_msg , ISocket* a_socket, IDecoder & a_decoder) override
    {
        std::cout << a_msg << std::endl;
    }

};


class TestHandler : public IHandler{
public:
    void Handle(std::string a_msg , ISocket* a_socket, IDecoder & a_decoder) override
    {
        std::cout << a_msg << std::endl;
        a_socket->Send("Message from server");
    }

};

class Test2Handler : public IHandler{
public:
    Test2Handler(EventRouter &a_eventTouter):m_eventRouter(a_eventTouter){}
    void Handle(std::string a_msg , ISocket* a_socket , IDecoder & a_decoder ) override
    {
        IEvent * event = a_decoder.Decode(a_msg);
        std::cout << a_msg << std::endl;
        std::shared_ptr<IEvent> sendEvent(event);
        m_eventRouter.SendEvent(sendEvent);
        a_socket->Send("Handled Event");
    }

private:
    EventRouter &m_eventRouter;

};

BEGIN_TEST(create_agent_from_so)
    SubscribersManager sub;
    ControllerHandler handler;
    EventDecoder decoder;
    const char * soname = "/home/leon/work/test/server/libSmoke.so";
    std::shared_ptr<IAgent> agent =  CreateAgent(soname, 1 ,"127.0.0.1" , 2010 , handler , sub , decoder);
    ASSERT_EQUAL(agent->GetId() , 1);

END_TEST

BEGIN_TEST(server)
    SubscribersManager sub;
    EventRouter eventRouter(sub , 20 , 2);
    ControllerHandler handler;
    EventTopic event("Smoke" , 2 ,205);
    EventTopic event2("Door" , 4 ,414);
    EventDecoder decoder;
    const char * soname = "/home/leon/work/test/server/libSmoke.so";
    std::shared_ptr<IAgent> smokeAgent =  CreateAgent(soname, 1 ,"127.0.0.1" , 2010 , handler , sub , decoder);
    MovementAgent * moveAgent = new MovementAgent(2,sub);
    std::shared_ptr<IAgent> agentM(moveAgent);
    Test2Handler testHandler(eventRouter);
    smokeAgent->Subscribe(event);
    moveAgent->Subscribe(event2);

    TcpServer server("127.0.0.1" , 2008 , testHandler , decoder);
    server.Run();
    ASSERT_PASS();

END_TEST


BEGIN_SUITE(Server)
    TEST(create_agent_from_so)
    TEST(server)

END_SUITE