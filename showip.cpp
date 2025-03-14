#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char* argv[]) {
    std::cout << "hello there: " << argv[1] << std::endl;
    
    int status;
    addrinfo hints, *res;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) {
        std::cerr << "put the hostname as the first argument\n";
        return 1;
    }

    memset(&hints, 0, sizeof(hints));   // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;        // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP stream sockets

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status);
        return 1;
    }

    std::cout << "IP addresses for " << argv[1] << ":\n\n";

    for (addrinfo* p = res; p != nullptr; p = p->ai_next) {
        void* addr;
        std::string_view ipver;
        int port;
        // get the pointer to the address itself
        // different fields in IPv4 and IPv6
        if (p->ai_family == AF_INET) {
            // IPv4
            sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in*>(p->ai_addr);
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
            port = ipv4->sin_port;
        }
        else {
            // IPv6
            sockaddr_in6 *ipv6 = reinterpret_cast<sockaddr_in6*>(p->ai_addr);
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
            port = ipv6->sin6_port;
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        std::cout << "\t" << ipver << ": " << ipstr << " port: " << port << "\n";


    }

    freeaddrinfo(res);

    return 0;
}