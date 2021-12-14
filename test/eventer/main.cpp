#include "mu_test.h"
#include "ievent.hpp"
#include "subscribers_manager.hpp"
#include "iagent.hpp"
#include "notifyer.hpp"


using namespace advcpp;

class SmokeEvent : public IEvent{
public:
    SmokeEvent(int a_floor , int a_room):IEvent(EventTopic("Smoke" , a_floor , a_room)){}

};

class DoorEvent : public IEvent{
public:
    DoorEvent(int a_floor , int a_room):IEvent(EventTopic("Door_Open" , a_floor , a_room)){}

};

class SmokeAgent : public IAgent , public std::enable_shared_from_this<SmokeAgent>{
public:
    SmokeAgent(size_t a_id , ISubscribers &a_subscriber):IAgent(a_id) , m_subscriber(a_subscriber){}

    void Notify(std::shared_ptr<IEvent> a_event) override
    {
        //std::cout << "got event - " << a_event->GetEventType() << std::endl;
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

class MovementAgent : public IAgent , public std::enable_shared_from_this<MovementAgent>{
public:
    MovementAgent(size_t a_id , ISubscribers &a_subscriber):IAgent(a_id) , m_subscriber(a_subscriber){}

    void Notify(std::shared_ptr<IEvent> a_event) override
    {
        //std::cout << "got event - " << a_event->GetEventType() << std::endl;
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

BEGIN_TEST(hash_test)
    SubscribersManager sub;
    EventTopic event("Smoke" , 2 ,205);
    SmokeAgent * smokeAgent = new SmokeAgent(1  , sub);
    std::shared_ptr<IAgent> agent(smokeAgent);
    agent->Subscribe(event);

    ASSERT_EQUAL(sub.HashSize() , 1);

END_TEST


BEGIN_TEST(find_in_hash)
    SubscribersManager sub;
    EventTopic event("Smoke" , 2 ,205);
    SmokeAgent * smokeAgent = new SmokeAgent(1,sub);
    std::shared_ptr<IAgent> agent(smokeAgent);
    agent->Subscribe(event);

    ASSERT_EQUAL(sub.IsDeviceSubscribe(event , agent) , true);

END_TEST

BEGIN_TEST(not_found_in_hash)
    SubscribersManager sub;
    EventTopic event("Smoke" , 2 ,205);
    SmokeAgent * smokeAgent = new SmokeAgent(1,sub);
    std::shared_ptr<IAgent> agent(smokeAgent);

    agent->Subscribe(event);
    EventTopic res("Light" , 2 ,205);

    ASSERT_EQUAL(sub.IsDeviceSubscribe(res , agent) , false);
    ASSERT_EQUAL(sub.HashSize() , 1);

END_TEST

BEGIN_TEST(insert_multiply_subscribers)
    SubscribersManager sub;
    EventTopic event("Smoke" , 2 ,205);
    EventTopic event2("Door" , 4 ,414);
    SmokeAgent * smokeAgent = new SmokeAgent(1,sub);
    std::shared_ptr<IAgent> agentS(smokeAgent);
    MovementAgent * moveAgent = new MovementAgent(2,sub);
    std::shared_ptr<IAgent> agentM(moveAgent);

    smokeAgent->Subscribe(event);
    smokeAgent->Subscribe(event2);
    moveAgent->Subscribe(event2);

    ASSERT_EQUAL(sub.IsDeviceSubscribe(event , agentS) , true);
    ASSERT_EQUAL(sub.IsDeviceSubscribe(event , agentM) , false);
    ASSERT_EQUAL(sub.IsDeviceSubscribe(event2 , agentS) , true);
    ASSERT_EQUAL(sub.IsDeviceSubscribe(event2 , agentM) , true);
    ASSERT_EQUAL(sub.GetSize() , 2);

END_TEST

BEGIN_TEST(unsubscribe)
    SubscribersManager sub;
    EventTopic event("Smoke" , 2 ,205);
    EventTopic event2("Door" , 4 ,414);
    SmokeAgent * smokeAgent = new SmokeAgent(1,sub);
    std::shared_ptr<IAgent> agentS(smokeAgent);
    MovementAgent * moveAgent = new MovementAgent(2,sub);
    std::shared_ptr<IAgent> agentM(moveAgent);


    smokeAgent->Subscribe(event);
    smokeAgent->Subscribe(event2);
    moveAgent->Subscribe(event2);

    smokeAgent->Unsubscribe(event2);

    ASSERT_EQUAL(sub.IsDeviceSubscribe(event , agentS) , true);
    ASSERT_EQUAL(sub.IsDeviceSubscribe(event , agentM) , false);
    ASSERT_EQUAL(sub.IsDeviceSubscribe(event2 , agentS) , false);
    ASSERT_EQUAL(sub.IsDeviceSubscribe(event2 , agentM) , true);
    ASSERT_EQUAL(sub.GetSize() , 2);

END_TEST

BEGIN_TEST(unsubscribe_not_found)
    SubscribersManager sub;
    EventTopic event("Smoke" , 2 ,205);
    EventTopic event2("Door" , 4 ,414);
    SmokeAgent * smokeAgent = new SmokeAgent(1,sub);
    std::shared_ptr<IAgent> agentS(smokeAgent);
    MovementAgent * moveAgent = new MovementAgent(2,sub);
    std::shared_ptr<IAgent> agentM(moveAgent);

    smokeAgent->Subscribe(event);
    smokeAgent->Subscribe(event2);
    moveAgent->Subscribe(event2);

    moveAgent->Unsubscribe(event);

    ASSERT_EQUAL(sub.IsDeviceSubscribe(event , agentS) , true);
    ASSERT_EQUAL(sub.IsDeviceSubscribe(event , agentM) , false);
    ASSERT_EQUAL(sub.IsDeviceSubscribe(event2 , agentS) , true);
    ASSERT_EQUAL(sub.IsDeviceSubscribe(event2 , agentM) , true);
    ASSERT_EQUAL(sub.GetSize() , 2);   


END_TEST

BEGIN_TEST(notify)
    SubscribersManager sub;
    Notifyer notifyer(sub);

    SmokeEvent * smokeEvent = new SmokeEvent(2 , 205);
    std::shared_ptr<IEvent> eventS(smokeEvent);
    DoorEvent * doorEvent = new DoorEvent(4 , 401);
    std::shared_ptr<IEvent> eventD(doorEvent);
    SmokeAgent * smokeAgent = new SmokeAgent(1,sub);
    std::shared_ptr<IAgent> agentS(smokeAgent);
    MovementAgent * moveAgent = new MovementAgent(2,sub);
    std::shared_ptr<IAgent> agentM(moveAgent);
    smokeAgent->Subscribe(eventS->GetEventTopic());
    smokeAgent->Subscribe(eventD->GetEventTopic());
    moveAgent->Subscribe(eventD->GetEventTopic());

    notifyer.Notify(eventS);
    ASSERT_THAT(smokeAgent->GetEventTopic() == eventS->GetEventTopic());
    notifyer.Notify(eventD);
    
    ASSERT_THAT(smokeAgent->GetEventTopic() == doorEvent->GetEventTopic());
    ASSERT_THAT(moveAgent->GetEventTopic() == doorEvent->GetEventTopic());

END_TEST
 

BEGIN_SUITE(eventer)
    TEST(hash_test)
    TEST(find_in_hash)
    TEST(not_found_in_hash)
    TEST(insert_multiply_subscribers)
    TEST(unsubscribe)
    TEST(unsubscribe_not_found)
    TEST(notify)

END_SUITE
