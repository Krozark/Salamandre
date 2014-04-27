#include <Salamandre-stats/broadcast.hpp>

namespace salamandre {
namespace stats {

Broadcast::Broadcast(int port, int listen_port)
{
    this->port = port;
    this->listen_port = listen_port;
}

void Broadcast::setupSocket()
{
    // If socket isn't setup
    if (this->socket <= 0) {
        this->socket = ::socket( PF_INET, SOCK_DGRAM, 0 );
        int broadcast_enable = 1;
        ::setsockopt(this->socket, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable));
        ::memset(&this->destination, '\0', sizeof(struct sockaddr_in));
        this->destination.sin_family = AF_INET;
        this->destination.sin_port = (in_port_t)::htons(this->port);
        this->destination.sin_addr.s_addr = ::htonl(INADDR_BROADCAST);
    }
}

void * Broadcast::serialize(Message message) {
    SerializedMessage * output = new SerializedMessage();
    strcpy(output->program_identifier, SALAMANDRE_PROGRAM_ID);
    output->type = htonl(message.type);
    output->data = htonl(message.data);

    return output;
}

Message Broadcast::deserialize(void * buffer) {
    SerializedMessage *input = (SerializedMessage *) buffer;
    Message message;
    if(strcmp(input->program_identifier, SALAMANDRE_PROGRAM_ID) != 0) {
        message.type = MESSAGE_INVALID;
        return message;
    }
    message.type = ntohl(input->type);
    message.data = ntohl(input->data);

    return message;
}

void Broadcast::send(Message message) {
    this->setupSocket();
    void * buffer = this->serialize(message);

    ::sendto(this->socket, buffer, sizeof(SerializedMessage), 0, (sockaddr*)&this->destination, sizeof(struct sockaddr_in));
}

void Broadcast::receive() {
    char buffer[32];
    Message message;
    struct sockaddr source;
    unsigned int source_length = sizeof(source);
    int listen_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = this->port;
    bind(listen_socket, (sockaddr*)&servaddr, sizeof(servaddr));
    int nbBytes;
    while (this->running) {
        nbBytes = ::recvfrom(listen_socket, buffer, sizeof(buffer), 0, &source, &source_length);
        if(nbBytes > 0) {
            message = this->deserialize(buffer);
        }
    }
    close(listen_socket);
}

void Broadcast::start() {
    // If the thread already started, not doing anything
    if (this->thread_receive.joinable()) {
        return;
    }
    this->running = true;
    this->thread_receive = std::thread(&Broadcast::receive, this);
}

void Broadcast::stop() {
    this->running = false;
    if(this->thread_receive.joinable()) {
        this->thread_receive.join();
    }
}


}
}
