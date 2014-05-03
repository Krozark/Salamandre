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
#include <Salamandre-stats/stats.hpp>


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
        uint32_t type;
        // For Presence
        uint32_t my_port;
        // For Recover
        uint32_t id_medecin;
        uint32_t id_patient;
        char filename[4];
    } Message;
    typedef struct serialized_message_stuct {
        char program_identifier[8];
        uint32_t type;
        // For Presence
        uint32_t my_port;
        // For Recover
        uint32_t id_medecin;
        uint32_t id_patient;
        char filename[4];
    } SerializedMessage;

    class Broadcast
    {
    public:
        Broadcast(int port, int listen_port);

        bool start();
        bool stop();
        void dispatch(Message message, std::string source_ip);
        void send(const Message message);

    private:
        bool setupSocket();
        void receive();
        SerializedMessage * serialize(const Message message);
        Message deserialize(void * buffer);

        int port;
        int listen_port;
        int listen_socket;
        int socket;
        bool running;
        struct sockaddr_in destination;
        std::thread thread_receive;
    };
}
}



#endif // BROADCAST_H
