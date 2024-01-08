#include "AllServers.hpp"

// AllServers::AllServers()
// {
//     // directiv_list={"server_name", "listen", "root", "index", "autoindex", "error_page", "client_max_body_size", "cgi", "allow_methods", "upload_dir", "return"};
//     // directiv_list = _directiv_list;

// }

size_t AllServers::find_server_end(std::string str)
{
    size_t sz = str.size();
    size_t posible_end = str.find("}");
    
    while(posible_end != sz - 1 && str[posible_end + 2] != 's')
    {
        posible_end = str.find_first_of("}",  posible_end + 1);
    }

    return posible_end;
}

void AllServers::make_lockations(std::string &s, Server &S)
{
    Map m;
    std::string tok;
    std::string key;
    std::string mkey;
    std::vector <std::string> value;
    std::vector<std::string> fv;
    std::stringstream ss(s);

    while(getline(ss, tok, ';'))
    {
        fv.push_back(tok);
        // std::cout <<line << std::endl;
    }
    for(int i = 0; i < fv.size(); ++i)
    {
        std::stringstream ss(fv[i]);
        while(ss >> key)
        {
            if(key == "location")
            {
                ss >> mkey; // to pass from "lockation" to mkey 
                ss >> tok; // to pass from "mkey" to "{"
                ss >> key; // to pass from "{" to next word key
            }
            
            value.clear();
            while(ss >> tok)
            {
                value.push_back(tok);
            }
            // newserv.insert(std::pair<std::string,  std::vector<std::string> >(key, value));
            if(key != "}")
                m.push_back(std::pair<std::string,  std::vector<std::string> >(key, value));
                // m[key]=value;
        }
    }
    // S.lockation[mkey] = m;
    S.lockation.push_back(std::pair<std::string, Map>(mkey, m));
}

Server const &AllServers::makeServer(std::string &s)
{

    Server *S = new Server;

    size_t ilockation = s.find("location");
    while(ilockation != std::string::npos)
    {
        size_t i_end_lockation = s.find("}");
        std::string str = s.substr(ilockation, i_end_lockation - ilockation + 1);
        make_lockations(str, *S);
        s.erase(ilockation, i_end_lockation - ilockation + 1);
        ilockation = s.find("location");
    }


    std::stringstream ss(s);
    int servisopen = 0;
    int lockisopen = 0;
    std::string tok;
    std::string key;
    std::vector <std::string> value;
    std::vector<std::string> fv;


    while(getline(ss, tok, ';'))
    {
        fv.push_back(tok);
        // std::cout <<tok << std::endl;
    }
    for(int i = 0; i < fv.size(); ++i)
    {
        std::stringstream ss(fv[i]);
        while(ss >> key)
        {
            if(key == "server")
            {
                ss >> key; // to pass from "server" to "{" 
                ss >> key; // to pass from "{" to next word key
            }
            
            value.clear();
            while(ss >> tok)
            {
                value.push_back(tok);
            }
            if(key != "}")
            {

                S->directives.push_back(std::pair<std::string,  std::vector<std::string> >(key, value));

            }
                // S->directives[key]=value;
        }
    }
    return *S;
            
}

// void AllServers::dir_is_valid(std::string dir, int from)
// {
//     directiv_list.find

// }
void AllServers::readConff()
{
    std::ifstream fin("config2.conf");
    std::string line;
    std::string full;
    std::string path;
    std::vector<std::string> fv;
  
    while(fin>>line)
    {
        full = full + " " + line;
    }

    while(0 < full.size())
    {
        servsCount++;
        size_t serv_end = find_server_end(full);
        std::string serv = full.substr(0, serv_end + 1);
        
        AllServs.insert(std::pair<int, Server>(servsCount, makeServer(serv)));
        full.erase(0, serv_end + 1);
    }    
}


void AllServers::print_vect(std::vector<std::string> &v)
{
    for(int i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;

}


void AllServers::print_map(Map m)
{
    std::cout << " LOCATION SIZE " << m.size() << " \n ";
    Map::iterator it = m.begin();
    for(; it != m.end(); ++it)
    {
        std::cout << "  " << it->first << " - ";
        print_vect(it->second);
    }
}

void AllServers::print_serv(int n)
{
    Map::iterator it = AllServs[n].directives.begin();
    std::cout << AllServs[n].directives.size() << " sizeeeeeeeee\n";
    for(; it != AllServs[n].directives.end(); ++it)
    {
        std::cout << "  " << it->first << " - ";
        print_vect(it->second);
    }

    std::vector<std::pair<std::string, Map>>::iterator i = AllServs[n].lockation.begin();
    for(; i != AllServs[n].lockation.end(); ++i)
    {
        // std::cout << "SONAAAAA I AM HERE !!! \n";

        std::cout << "Lockationnn  " << i->first << std::endl;
        print_map(i->second);
    }

}
void AllServers::print_AllServs()
{
    for(int i = 0; i < servsCount; ++i)
    {
        std::cout << "Server N - " << i+1 <<" is :\n";
        print_serv(i + 1);
    }
}