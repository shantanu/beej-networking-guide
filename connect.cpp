#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main ()
{
    struct addrinfo hints, *res;
    int sockfd;

    // fill in the hints and call getaddrinfo;  
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo("www.google.com", "80", &hints, &res);

    // make a socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // we don't need to call bind here! 
    // the OS will assign a port by itself (this is OK since we are a TCP client)

    // connect!
    connect(sockfd, res->ai_addr, res->ai_addrlen);
}