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
        std::cerr << "(Socket error)" << std::endl;
        exit(1);
    }

    Socket socket(sockfd);

    int i_socket_option = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *) &i_socket_option,
                   sizeof(i_socket_option)) == -1) {
        std::cerr << "(Set socket option error)" << std::endl;
        exit(1);
    }

    sockaddr_in addr;
    int port = 8080;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (sockaddr *) &addr, sizeof(addr)) != 0) {
        std::cerr << "(Socket bind failed)" << std::endl;
        exit(1);
    }

    if (::listen(sockfd, SOMAXCONN) != 0) {
        std::cerr << "(Couldn't listen)" << std::endl;
        exit(1);
    }

    return socket;
}

void Server::worker(void)
{
    for (;;) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int clientfd =
            accept(socket.fd(), (sockaddr *) &client_addr, &client_len);

        if (clientfd < 0) {
            std::cerr << "(Accept error)" << std::endl;
            exit(1);
        }

        Request req((Socket(clientfd)));
        bool corrupt = req.handler();

        if (corrupt == true)
            // This causes failed requests when testing
            // because it doesn't responsd with anything
            continue;

        Response res((Socket(clientfd)));
        res.handler(req.method, req.target);
    }
}
