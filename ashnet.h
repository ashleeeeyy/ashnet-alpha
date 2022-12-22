//
// Created by Ashley on 12/19/22.
//
#pragma once

#include <string>
#include <cstdint>
#include <arpa/inet.h>
namespace ashnet {

    class Address4 {
    public:
        //creates IPv4 address from string representation
        explicit Address4(const std::string& addr_string);

        uint32_t as_network_int();
        uint32_t as_host_int();
        std::string as_string();

    private:
        //by default should be in network byte order
        uint32_t raw_addr_4;

    };

    class Endpoint4 {
    public:
        Endpoint4(const std::string &addr_string, int port);
        sockaddr_in to_sockaddr_in();

        Address4 address;
        int port;
    };

    class NetworkException : std::exception{
    public:
        enum ErrorCode {
            SOCKET_INIT_ERROR = 1
        };
        ErrorCode code;
        explicit NetworkException(ErrorCode e);
    };

    enum Protocol {
        TCP = 1, UDP = 2
    };
    class NetworkSocket {
    public:
        Protocol protocol;
        int socket_fd;
        //
        // Creates NetworkSocket object and attempts to initialize a socket
        //
        explicit NetworkSocket(Protocol aProtocol);
        //
        // Closes socket FD
        //
        ~NetworkSocket();

        int bind_to(Endpoint4);
    };
}
