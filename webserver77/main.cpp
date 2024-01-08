#include "AllServers.hpp"

int main()
{
    
        /* try
        {
            ParsConf kk;
            kk.readConf();
            // kk.readServer();
            kk.print_map();
          
        }
        catch(const std::exception& e)
        {
            std::cout << "hhhhh" << std::endl;
            std::cerr << e.what() << '\n';
        } */
    // std::vector <std::string> til;
    // til.push_back("listen 1-gfrdg 2-erge  3-erygrg");

  AllServers k;
//   k.check_till_end(til);
    k.readConff();
    k.print_AllServs();
    // std::cout << k.servsCount;
    // k.print_servs();
}