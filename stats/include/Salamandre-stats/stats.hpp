#ifndef STATS_HPP
#define STATS_HPP

#include <iostream>
#include <list>
#include <time.h>
#include <Salamandre-stats/node.hpp>

class Stats
{
    public:
        Stats() = delete;

        static void add_node(std::string host, int port);
        static void delete_node(std::string host, int port);

        /**
         * \brief get a number of nodes
         */
        static std::list<std::shared_ptr<Node>> get_nodes(unsigned int number);

        static std::list<std::shared_ptr<Node>> get_nodes();
};

#endif // STATS_HPP
