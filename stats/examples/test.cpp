
#include <Salamandre-stats/stats.hpp>
#include <iostream>
#include <thread>
#include <chrono>


using namespace salamandre::stats;

int main(int argc, char * argv[])
{
    Stats::init();

    Stats::flush_nodes();

    if (argc > 1)
    {
        std::cout << "Loading from " << argv[1] << "..." << std::endl;
        Stats::load(argv[1]);
        for(auto& u : Stats::get_nodes(Stats::get_duplication_number_for(99.99)))
            std::cout<<*u<<std::endl;

    } else {
        Stats::add_node(std::string("test"), 1234);
        Stats::add_node(std::string("test2"), 4567);
        Stats::add_node(std::string("test3"), 7890);
        Stats::add_node(std::string("test"), 1234);

        for(auto& u : Stats::get_nodes(Stats::get_duplication_number_for(99.99)))
            std::cout<<*u<<std::endl;
    }

    Stats::flush_nodes();

    Stats::close();
    return 0;
}
