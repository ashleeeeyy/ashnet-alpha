#include <iostream>
#include "include/ashnet.h"
#include <array>
#include <chrono>
int main() {

    std::cout << "Starting test..." << std::endl;
    try{

        ashnet::Endpoint<ashnet::Internet4> endpoint("0.0.0.0", 9123);
        std::cout << "test binding on "
                  << endpoint.address.as_string() << std::endl;

        ashnet::DgramSocket<ashnet::Internet4> sock;
        sock.listen(endpoint);

        std::cout << "bound" << std::endl;

        std::array<std::byte, 1024> buffy{};
        int read = 0;
        while (1) {
            auto sender = sock.read(buffy);
            auto start = std::chrono::high_resolution_clock::now();
            read += 1024;
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
            std::cout << "\rread " << read << " bytes from " << sender.address.as_string()
            << ":" << sender.port << " took " << duration.count() << "us" <<std::flush;
        }

    }catch (ashnet::NetworkException &exception){
        std::cerr << "error creating and binding socket: " << exception.get_message() << std::endl;
    }

    return 0;
}
