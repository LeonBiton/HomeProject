#include "iagent.hpp"

using namespace advcpp;

IAgent::IAgent(size_t a_id)
: m_id(a_id){}


size_t IAgent::GetId() const
{
    return m_id;
}
