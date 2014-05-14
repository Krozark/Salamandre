#ifndef STATS_HPP
#define STATS_HPP

#include <iostream>
#include <list>
#include <time.h>
#include <Salamandre-stats/node.hpp>

namespace salamandre {

    namespace stats {
        class Stats
        {
            public:
                Stats() = delete;

                static float default_robustesse;

                static void add_node(std::string host, int port);
                static void delete_node(std::string host, int port);
                // Deletes all nodes
                static void flush_nodes();

                static void init();
                static void close();


                /**
                 * \brief get the number of minimal duplication of file to have
                 * a robustesse of \a robustesse %
                 * \param robustesse the robustess needed (0~100)
                 * \return the number of duplication neded
                 */
                static unsigned int get_duplication_number_for(float robustesse=default_robustesse);

                /**
                 * \brief get a number of nodes
                 */
                static std::list<std::shared_ptr<Node>> get_nodes(unsigned int number=get_duplication_number_for());

                /**
                 * @brief load nodes from a file
                 * @param path Path of the file containing node list
                 * @return true if success, false otherwise
                 */
                static bool load(const char * path);

            private:
        };
    }

}
#endif // STATS_HPP
