#include "AllServers.hpp"
#include "Server.hpp"

Server::Server()
{
    
}


Server::~Server()
{

}

void Server::add_locations(std::pair<std::string, Directives> &p)
{
    _locations.push_back(p);
}



std::vector<std::pair<std::string, Directives> > const & Server::get_locations() const
{
    return (_locations);
}


