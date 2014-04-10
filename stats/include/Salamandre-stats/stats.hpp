#ifndef STATS_HPP
#define STATS_HPP

#include <iostream>
#include <list>
#include <time.h>
#include <Salamandre-stats/node.hpp>

class Stats
{
public:
    Stats();
    void setup();
    void add_node(std::string host, int port);
    void delete_node(std::string host, int port);
    std::list<std::shared_ptr<Node>> get_nodes();
};

#endif // STATS_HPP
