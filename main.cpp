#include "server/Server.hpp"

int main(int ac, char **av){
    if (ac > 2)
    {
        std::cout << "invalid: many parametr av" << std::endl;
        return (0);
    }    
    AllConfigs configs;
    try{
        if(ac == 2)
            configs._st = av[1];
        configs.readConff();
        configs.chech_directive();
    }
    catch(const std::exception& e)
    {
        // std::cout << "hhhhh" << std::endl;
        std::cerr << e.what() << std::endl;;
        return (0);
    }
    catch(...)
    {
        std::cout << "hhhhh" << std::endl;
    }
    try{
        Server server1(configs._AllServs);
        server1.launchServer();
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
    }
     catch(...)
    {
        std::cout << "Some error" << std::endl;
    }
    return 0;
}