#ifndef DISCOVER_HPP
#define DISCOVER_HPP

#include <iostream>
#include <list>
#include <thread>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <Salamandre-stats/node.hpp>

namespace salamandre {
    namespace stats {
        class Discover
        {
            public:
                Discover(int listen_port);
                bool start();
                bool stop();
                void send();
            private:
                std::thread thread_send;
                int target_port = 24532;
                int listen_port;
                int socket = 0;
                bool running = false;

        };

    }
}
#endif // DISCOVER_HPP
