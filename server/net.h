#ifndef NET_H
#define NET_H

#include <utility>

class Socket {
public:
    explicit Socket(int fd)
        : fd_(fd)
    {
    }
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&& o) noexcept
        : fd_(std::exchange(o.fd_, -1))
    {
    }

    int fd() const
    {
        return fd_;
    }

private:
    int fd_ = -1;
};

Socket init_server_sock();

class Server {
public:
    explicit Server(void)
        : socket(init_server_sock())
    {
    }
    void run();

private:
    Socket socket;
};

#endif
