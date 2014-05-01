#include <Salamandre-stats/broadcast.hpp>

namespace salamandre {
namespace stats {

Broadcast::Broadcast(int port, int listen_port)
{
    this->port = port;
    this->listen_port = listen_port;
    std::cout << "Broadcast initialized on port: " << port << " with listen_port: " << listen_port << std::endl;
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
        std::cout << "Socket initialized on port " << this->port << std::endl;
    }
}

SerializedMessage * Broadcast::serialize(const Message message) {
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

void Broadcast::send(const Message message) {
    this->setupSocket();
    SerializedMessage * buffer = this->serialize(message);

    ::sendto(this->socket, buffer, sizeof(SerializedMessage), 0, (sockaddr*)&this->destination, sizeof(struct sockaddr_in));

    std::cout << "Message of type: " << message.type << " with data: " << message.data << " sent." << std::endl;

    delete buffer;
}

void Broadcast::receive() {

    std::cout << "Thread receiving started" << std::endl;
    char buffer[sizeof(SerializedMessage)];
    Message message;
    struct sockaddr_in source;
    unsigned int source_length = sizeof(source);
    char source_ip[INET_ADDRSTRLEN];
    this->listen_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htonl(this->port);
    if(bind(this->listen_socket, (sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Unable to bind: ");
        close(this->listen_socket);
        return;
    }
    int nbBytes;
    while (this->running) {
        nbBytes = ::recvfrom(listen_socket, buffer, sizeof(buffer), 0, (sockaddr *)&source, &source_length);
        if(nbBytes > 0) {
            message = this->deserialize(buffer);
            ::inet_ntop(AF_INET, &(source.sin_addr), source_ip, INET_ADDRSTRLEN);

            std::cout << "Message of type: " << message.type << " with data: " << message.data << " received from: " << source_ip << std::endl;

            this->dispatch(message, std::string(source_ip));
        }
    }
    close(this->listen_socket);
    std::cout << "Thread receiving stopped" << std::endl;
}

void Broadcast::dispatch(Message message, std::string source_ip) {
    switch(message.type) {
        case MESSAGE_PRESENCE:
            Stats::add_node(source_ip, message.data);
            break;
        case MESSAGE_RECOVER:
            break;
        case MESSAGE_INVALID:
        default: // Invalid message
            break;
    }
}

void Broadcast::start() {
    // If the thread already started, not doing anything
    if (this->thread_receive.joinable()) {
        std::cerr << "Thread receiving already active, not doing anything" << std::endl;
        return;
    }
    std::cout << "Starting receiving thread..." << std::endl;
    this->running = true;
    this->thread_receive = std::thread(&Broadcast::receive, this);
}

void Broadcast::stop() {
    std::cout << "Stopping receiving thread..." << std::endl;
    this->running = false;
    ::shutdown(this->listen_socket, SHUT_RDWR);
    if(this->thread_receive.joinable()) {
        this->thread_receive.join();
    }
}


}
}
