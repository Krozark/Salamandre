
#include <Salamandre-stats/stats.hpp>

//#include <iostream>


using namespace salamandre::stats;

int main()
{
    Stats::init();

    Stats::add_node(std::string("test"), 1234);

    for(auto& u : Stats::get_nodes(Stats::get_duplication_number_for(99.99)))
        std::cout<<*u<<std::endl;

    Stats::close();
    return 0;
}
