//
// Created by Ashley on 12/19/22.
//

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <array>
#include <utility>
#include "../include/ashnet.h"

ashnet::NetworkException::NetworkException(int code) : code(code){}
ashnet::NetworkException::NetworkException(std::string message) : message(std::move(message)){}

std::string ashnet::NetworkException::get_message() {
    if (message.empty()) {
        return std::strerror(code);
    }else {
        return message;
    }
}


