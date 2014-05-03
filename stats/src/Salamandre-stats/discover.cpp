#include <Salamandre-stats/discover.hpp>

using namespace salamandre::stats;

Discover::Discover(int listen_port) :target_port(24532), listen_port(listen_port), broadcast(target_port, listen_port), socket(0), running(false) {
    std::cout << "Discover initialized" << std::endl;
}


void Discover::send() {
    std::cout << "Thread sending started" << std::endl;
    Message message;
    message.type = MESSAGE_PRESENCE;
    message.my_port = this->listen_port;

    std::chrono::seconds duration(30);
    while(this->running) {
        this->broadcast.send(message);
        std::this_thread::sleep_for(duration);
    }

    std::cout << "Thread sending stopped" << std::endl;
}

bool Discover::start() {
    // If the thread already started, not doing anything
    if (this->thread_send.joinable()) {
        std::cerr << "Thread sending already started, not doing anything" << std::endl;
        return false;
    }
    std::cout << "Starting sending thread..." << std::endl;
    this->running = true;
    this->thread_send = std::thread(&Discover::send, this);
    this->broadcast.start();

    return true;
}

bool Discover::stop() {
    std::cout << "Stopping sending thread..." << std::endl;
    this->running = false;
    if(this->thread_send.joinable()) {
        this->thread_send.join();
    }
    this->broadcast.stop();

    return true;
}
