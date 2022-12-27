#include <cstdint>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

//
// Created by Laptop on 12/22/22.
//

// ASHNET::ADDRESS IPV4
inline ashnet::Address<ashnet::Internet4>::Address(const std::string &addr_string)
        : raw_addr(0) {
    inet_pton(AF_INET, addr_string.c_str(), &this->raw_addr);
}

inline ashnet::Address<ashnet::Internet4>::Address(uint32_t raw_addr_4)
        : raw_addr(raw_addr_4) {}

inline uint32_t ashnet::Address<ashnet::Internet4>::as_host_int() {
    return htonl(raw_addr);
}

inline uint32_t ashnet::Address<ashnet::Internet4>::as_network_int() {
    return raw_addr;
}

inline std::string ashnet::Address<ashnet::Internet4>::as_string() {
    char buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &raw_addr, buffer, INET_ADDRSTRLEN);
    std::string str(buffer);
    return str;
}

template<class T>
ashnet::Endpoint<T>::Endpoint(Address <T> address, int port) :
address(address), port(port) {}

template<class T>
ashnet::Endpoint<T>::Endpoint(const std::string &addr_string, int port) :
address(Address<T>(addr_string)), port(port) {}

template<>
inline ashnet::Endpoint<ashnet::Internet4>::Endpoint(sockaddr *sockaddr) :
address((reinterpret_cast<sockaddr_in *>(sockaddr))->sin_addr.s_addr),
port(reinterpret_cast<sockaddr_in *>(sockaddr)->sin_port) {}

template<>
inline sockaddr ashnet::Endpoint<ashnet::Internet4>::get_sockaddr() {
    sockaddr_in sa_in{};
    sa_in.sin_port = htons(port);
    sa_in.sin_addr.s_addr = address.as_network_int();
    sa_in.sin_family = AF_INET;

    //ptr re-interpretation
    auto *sa_store = (sockaddr *) &sa_in;
    return *sa_store;
}

template<>
template<std::size_t SIZE>
ashnet::Endpoint<ashnet::Internet4> ashnet::DgramSocket<ashnet::Internet4>::read(std::array<std::byte, SIZE> &buffer) {
    sockaddr_in recv_from_addr{};
    socklen_t recv_from_addr_len = sizeof(sockaddr_in);

    auto sockaddr_ptr = reinterpret_cast<sockaddr *>(&recv_from_addr);
    recvfrom(socket_fd, &buffer, SIZE, 0, sockaddr_ptr, &recv_from_addr_len);
    Endpoint<Internet4> sender_endpoint(sockaddr_ptr);

    return sender_endpoint;
}

template<>
inline ashnet::DgramSocket<ashnet::Internet4>::DgramSocket() : is_bound(false) {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        throw ashnet::NetworkException(errno);
    }
    socket_fd = sock_fd;
}

template<class T>
ashnet::DgramSocket<T>::~DgramSocket() {
    close(socket_fd);
}

template<class T>
void ashnet::DgramSocket<T>::listen(Endpoint <T> endpoint) {
    if (is_bound) {
        throw NetworkException(EISCONN);
    }
    sockaddr sa = endpoint.get_sockaddr();
    int bind_code = bind(socket_fd, &sa, sizeof(sa));
    if (bind_code == -1) {
        throw ashnet::NetworkException(errno);
    }
    is_bound = true;
}

template <>
ashnet::StreamServer<ashnet::Internet4>::StreamServer() : is_bound(false) {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        throw ashnet::NetworkException(errno);
    }
    socket_fd = sock_fd;
}

template <class T>
ashnet::StreamServer<T>::~StreamServer() {
    close(socket_fd);
}

template <>
void ashnet::StreamServer<ashnet::Internet4>::start(Endpoint <Internet4> endpoint) {
    if (is_bound) {
        throw NetworkException(EISCONN);
    }
    sockaddr sa = endpoint.get_sockaddr();
    int bind_code = bind(socket_fd, &sa, sizeof(sa));
    if (bind_code == -1) {
        throw ashnet::NetworkException(errno);
    }

    int listen_code = listen(socket_fd, 1);
    if (listen_code == -1) {
        throw ashnet::NetworkException(errno);
    }

    is_bound = true;
}
template<class T>
void ashnet::StreamServer<T>::start_accept_loop() {

}
