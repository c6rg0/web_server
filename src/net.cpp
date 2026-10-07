#include "net.h"
#include "response.h"
#include <iostream>
#include <string>
#include <fcntl.h>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h> // open(), write(), close()

Network::Network (void)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        std::cout << "(Socket error)\n";
        exit(1);
    }

    sockaddr_in serv_addr;
    int port = 8080;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
        std::cout << "(Socket bind failed)\n";
        exit(1);
    }
}

void Network::listen (void)
{
    if (::listen(sockfd, SOMAXCONN) != 0) {
        std::cout << "(Couldn't listen)\n";
        exit(1);
    }

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    clientfd = accept(sockfd, (sockaddr *)&client_addr, &client_len);

    if (clientfd < 0) {
        std::cout << "(Accept error)\n";
        exit(1);
    }

    std::cout << "\n(Accept success)\n";
}

void Network::read (char* buffer)
{
    ssize_t request = ::read(clientfd, buffer, 8192);

    if (request <= 0) {
        std::cout << "(Request contains no data or read error)\n";
        exit(1);
    }
}

void Network::write(std::string payload)
{
    ssize_t response;
    response = ::write(clientfd, payload.data(), payload.size());
    if (response < 0) {
        std::cout << "(Data wasn't written to target)\n";
        exit(1);
    }
    std::cout << "(Data sent to target)\n";
}

void Network::close(void)
{
    if (clientfd)
        ::close(clientfd);
    if (sockfd)
        ::close(sockfd);
}

// Trigered at the end of each main() while loop
Network::~Network()
{
    if (clientfd)
        ::close(clientfd);
}
