#ifndef ALLSERVERS_HPP
#define ALLSERVERS_HPP

#include "Server.hpp"

class AllServers
{
public:
    int servsCount;
    std::map<int, Server> AllServs;
    std::vector<std::string> directiv_list;

    void readConff();
    size_t find_server_end(std::string str);
    void make_lockations(std::string &s, Server &S);
    Server const &makeServer(std::string &s);
    void print_AllServs();
    void print_serv(int n);
    void print_vect(std::vector<std::string> &v);
    void print_map(Map);

    //CHeck

    // void dir_is_valid(std::string dir, int from);

};


#endif