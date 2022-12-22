//
// Created by Ashley on 12/19/22.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ashnet.h"

ashnet::NetworkSocket::NetworkSocket(Protocol aProtocol) {
    this->protocol = aProtocol;
    int sock_fd = socket(AF_INET, aProtocol, 0);
    if (sock_fd < 0) {
        throw NetworkException(NetworkException::SOCKET_INIT_ERROR);
    }
    socket_fd = sock_fd;
}
ashnet::NetworkSocket::~NetworkSocket() {
    close(socket_fd);
}

int ashnet::NetworkSocket::bind_to(ashnet::Endpoint4 endpoint) {

    sockaddr_in in_sa = endpoint.to_sockaddr_in();
    auto *addr = reinterpret_cast<struct sockaddr*>(&in_sa);
    int bind_res = bind(socket_fd, addr, sizeof(struct sockaddr_in));
    return bind_res;
}

ashnet::NetworkException::NetworkException(ErrorCode e) {
    this->code = e;
}

ashnet::Address4::Address4(const std::string& addr_string) : raw_addr_4(0) {
    inet_pton(AF_INET, addr_string.c_str(), &this->raw_addr_4);
}

uint32_t ashnet::Address4::as_network_int() {
    return raw_addr_4;
}

uint32_t ashnet::Address4::as_host_int() {
    return htonl(raw_addr_4);
}

std::string ashnet::Address4::as_string() {
    char buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &raw_addr_4, buffer, INET_ADDRSTRLEN);
    std::string str(buffer);
    return str;
}

ashnet::Endpoint4::Endpoint4(const std::string &addr_string, int port) : address(addr_string) {
    this->port = port;
}

sockaddr_in ashnet::Endpoint4::to_sockaddr_in() {
    struct sockaddr_in sa {
        0,
        AF_INET,
        static_cast<in_port_t>(port),
        address.as_network_int(),
        0,
    };
    sa.sin_len = sizeof(sockaddr_in);
    return sa;
}
