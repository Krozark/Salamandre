#ifndef BROADCAST_H
#define BROADCAST_H

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

namespace salamandre {
namespace stats {

    // Program identifier
    const char SALAMANDRE_PROGRAM_ID[8] = "Salaman";
    enum MESSAGE_TYPE:unsigned int {
        MESSAGE_INVALID = 0,
        MESSAGE_PRESENCE = 10,
        MESSAGE_RECOVER = 20
    };
    typedef struct message_struct {
        unsigned int type;
        int data;
    } Message;
    typedef struct serialized_message_stuct {
        char program_identifier[8];
        unsigned int type;
        int data;
    } SerializedMessage;

    class Broadcast
    {
    public:
        Broadcast(int port, int listen_port);

        void start();
        void stop();
        void dispatch();
        void send(Message message);

    private:
        void setupSocket();
        void receive();
        void * serialize(Message message);
        Message deserialize(void * buffer);

        int port;
        int listen_port;
        std::thread thread_receive;
        int socket = 0;
        bool running = false;
        struct sockaddr_in destination;
        char message[64];
    };
}
}



#endif // BROADCAST_H
