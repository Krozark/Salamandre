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

        /**
         * \brief get the number of minimal duplication of file to have 
         * a robustesse of \a robustesse %
         * \param robustesse the robustess needed (0~100)
         * \return the number of duplication neded
         */
        static unsigned int get_duplication_number_for(float robustesse);

    private:
        static std::list<std::shared_ptr<Node>> get_nodes();
};

#endif // STATS_HPP
