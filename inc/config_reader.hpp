#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP
#include <fstream>
#include <memory>
#include <vector>
#include "iagent.hpp"


class ConfigReader{
public:
    ConfigReader(std::string a_fileName);

    std::vector<std::shared_ptr<IAgent>> Extract();

private:
    std::ifstream m_file; 
    std::vector<std::shared_ptr<IAgent>> m_agents;

};

#endif