#include "subscribers_manager.hpp"

void SubscribersManager::Subscribe(EventTopic a_eventTopic, std::shared_ptr<IAgent> a_agent)
{
    auto it = m_hash.find(a_eventTopic);
    if(it == m_hash.end())
    {
        m_hash.insert({a_eventTopic , agentVector{a_agent}});
    }
    else
    {
        (it->second).push_back(a_agent);
    }
}

bool SubscribersManager::IsDeviceSubscribe(EventTopic a_eventTopic, std::shared_ptr<IAgent> a_agent) const
{
    auto it = m_hash.find(a_eventTopic);
    if(it == m_hash.end())
    {
        return false;
    }
    return std::find((it->second.begin()), (it->second.end()), a_agent) != (it->second).end();
}


size_t SubscribersManager::HashSize() const
{
    return m_hash.size();
}

size_t SubscribersManager::GetSize() const
{
    return m_hash.size();
}

void SubscribersManager::Unsubscribe(EventTopic a_eventTopic, std::shared_ptr<IAgent> a_agent)
{
    auto it = m_hash.find(a_eventTopic);
    if(it == m_hash.end())
    {
        return;
    }
    agentVector::iterator vectorItr = (it->second).begin();
    while(vectorItr != ((it->second).end()))
    {
        if((*vectorItr) == a_agent)
        {
            break;
        }
        ++vectorItr;
    }
    if(vectorItr == (it->second).end())
    {
        return;
    }
    (it->second).erase(vectorItr);
}

agentVector SubscribersManager::FindSubscribers(EventTopic a_eventTopic)
{
    auto it = m_hash.find(a_eventTopic);
    if(it == m_hash.end())
    {
        throw std::runtime_error("subscriber not found");
    }
    return it->second;
}
