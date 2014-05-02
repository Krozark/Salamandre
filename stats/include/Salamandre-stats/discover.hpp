#ifndef DISCOVER_HPP
#define DISCOVER_HPP

#include <iostream>
#include <list>
#include <thread>
#include <chrono>
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
#include <Salamandre-stats/broadcast.hpp>

namespace salamandre {
    namespace stats {
        class Discover
        {
            public:
                Discover(int listen_port);
                ~Discover();
                bool start();
                bool stop();
                void send();
            private:
                int target_port;
                int listen_port;
                Broadcast broadcast;
                int socket;
                bool running;
                std::thread thread_send;

        };

    }
}
#endif // DISCOVER_HPP
