#include "config_reader.hpp"

ConfigReader::ConfigReader(std::string a_fileName)
:m_file()
,m_agents()
{
    m_file.open(a_fileName);
}

std::vector<std::shared_ptr<IAgent>> ConfigReader::Extract()
{
    std::string buffer;   
    getline(m_file , buffer);
}
