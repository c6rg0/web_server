#include "net.h"
#include "request.h"
#include "response.h"
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h> // open(), write(), close()

Socket::~Socket()
{
    if (fd_ >= 0)
        close(fd_);
}

Socket init_server_sock()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        std::cout << "(Socket error)\n";
        exit(1);
    }

    Socket socket(sockfd);

    sockaddr_in addr;
    int port = 8080;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (sockaddr *) &addr, sizeof(addr)) != 0) {
        std::cout << "(Socket bind failed)\n";
        exit(1);
    }

    if (::listen(sockfd, SOMAXCONN) != 0) {
        std::cout << "(Couldn't listen)\n";
        exit(1);
    }

    return socket;
}

void Server::run(void)
{
    for (;;) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int clientfd =
            accept(socket.fd(), (sockaddr *) &client_addr, &client_len);

        if (clientfd < 0) {
            std::cout << "(Accept error)\n";
            exit(1);
        }

        Request req((Socket(clientfd)));
        req.handler();

        Response res((Socket(clientfd)));
        res.handler(req.method, req.target);
    }
}
