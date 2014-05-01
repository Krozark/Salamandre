
#include <Salamandre-stats/stats.hpp>
#include <Salamandre-stats/discover.hpp>
#include <iostream>
#include <thread>
#include <chrono>


using namespace salamandre::stats;

int main()
{
    Stats::init();

    Stats::add_node(std::string("test"), 1234);

    for(auto& u : Stats::get_nodes(Stats::get_duplication_number_for(99.99)))
        std::cout<<*u<<std::endl;

    Discover discover(1234);

    discover.start();
    std::chrono::seconds duration(120);
    std::this_thread::sleep_for(duration);
    discover.stop();

    Stats::close();
    return 0;
}
