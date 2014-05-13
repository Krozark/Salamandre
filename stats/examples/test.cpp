
#include <Salamandre-stats/stats.hpp>
#include <iostream>
#include <thread>
#include <chrono>


using namespace salamandre::stats;

int main()
{
    Stats::init();
    Stats::flush_nodes();

    Stats::add_node(std::string("test"), 1234);
    Stats::add_node(std::string("test"), 4567);
    Stats::add_node(std::string("test"), 1234);

    for(auto& u : Stats::get_nodes(Stats::get_duplication_number_for(99.99)))
        std::cout<<*u<<std::endl;

    Stats::close();
    return 0;
}
