#include <iostream>
#include "ashnet.h"
#include <cerrno>

int main() {

    std::cout << "Starting test..." << std::endl;
    try{
        ashnet::NetworkSocket net_sock(ashnet::Protocol::TCP);

        ashnet::Endpoint4 endpoint("0.0.0.0", 8080);
        std::cout << "test binding on "
        << endpoint.address.as_string() << std::endl;

        int test = net_sock.bind_to(endpoint);
        if (test < 0){
            std::cout << "Error creating binding: "
            << std::strerror(errno)
            << std::endl;
        }

    }catch (ashnet::NetworkException &exception){
        std::cerr << "error creating socket: " << exception.code << std::endl;
    }

    return 0;
}
