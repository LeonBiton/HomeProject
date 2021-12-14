#include "iagent.hpp"
#include "isubscriber.hpp"

extern "C"
IAgent * MakeAgent(size_t a_id , std::string a_ip , int a_port , IHandler &a_handler,
 ISubscribers &a_subscriber , IDecoder & a_decoder);

extern "C"
class SmokeAgent : public IAgent , public std::enable_shared_from_this<SmokeAgent>{
public:
    SmokeAgent(size_t a_id , std::string a_ip , int a_port , IHandler &a_handler ,
 ISubscribers &a_subscriber , IDecoder & a_decoder);

    void Notify(std::shared_ptr<IEvent> a_event) override;
    void Subscribe(EventTopic a_eventTopic) override;
    void Unsubscribe(EventTopic a_eventTopic) override;

    EventTopic GetEventTopic() const;

private:
    EventTopic m_eventReceived;
    ISubscribers &m_subscriber;
    TcpClient m_server;
};