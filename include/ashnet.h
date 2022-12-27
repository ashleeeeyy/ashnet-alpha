//
// Created by Ashley on 12/19/22.
//
#pragma once

#include <string>
#include <cstdint>
#include <arpa/inet.h>

namespace ashnet {

    class Protocol {
            const static int addr_len;
    };
    class Internet4 : public Protocol{
        public:
            const static int addr_len = INET_ADDRSTRLEN;
    };
    class Internet6 {

    };

    template <class T>
    class Address {
            static_assert(std::is_base_of<Protocol, T>(), "Template type must have a base of Protocol");
    };

    template <>
    class Address<Internet4> {
        public:
            //creates IPv4 address from string representation
            explicit Address<>(const std::string &addr_string);
            explicit Address<>(uint32_t raw_addr_4);

            //get raw address in network byte order (big endian)
            uint32_t as_network_int();

            //get raw address in host byte order (little endian)
            uint32_t as_host_int();

            //get IP address in standard string representation (ex: 10.0.0.1)
            std::string as_string();

        private:
            //by default should be in network byte order
            uint32_t raw_addr;
    };
    template <class T>
    class Endpoint {
            static_assert(std::is_base_of<Protocol, T>(), "Template type must have a base of Protocol");
        public:
            Endpoint(const std::string &addr_string, int port);
            Endpoint(Address<T> address, int port);

            Endpoint(sockaddr* sockaddr);

            sockaddr get_sockaddr();

            Address<T> address;
            int port;
    };



    class NetworkException : std::exception {
        public:
            explicit NetworkException(int code);
            explicit NetworkException(std::string message);
            std::string get_message();
        private:
            std::string message;
            int code;
    };


    //TODO: Move this into tcp and udp specific classes!
    //tcp will have a server, client, etc...
    //udp will only have a socket which can be directly read from / wrote to

    //tcp will have a server which will dispatch connections to a callback and contain
    //management capabilities for each connection


    template <class T>
    class DgramSocket{
        public:
            DgramSocket();
            ~DgramSocket();

            void listen(Endpoint<T> endpoint);

            template<std::size_t SIZE>
            Endpoint<T> read(std::array<std::byte, SIZE> &buffer);
        private:
            int socket_fd;
            bool is_bound;
    };

    template <class T>
    class StreamSocket{
        public:
            StreamSocket();
            ~StreamSocket();

            void start(Endpoint<T> endpoint);
        private:
            void start_accept_loop();
            int socket_fd;
            bool is_bound;
    };

}
#include "ashnet.tpp"
