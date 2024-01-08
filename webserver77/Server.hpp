#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>

typedef std::vector<std::pair<std::string,  std::vector<std::string>>> Map;

class Server 
{
public:
    Server();
    ~Server();
    
    Map directives;
    std::vector<std::pair<std::string, Map>> lockation;
};







#endif