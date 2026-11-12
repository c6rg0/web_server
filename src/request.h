#ifndef REQUEST_H
#define REQUEST_H

#include "server.h"
#include <string>

class Request {
public:
    // TODO: Should the below be a map called tokens?
    std::string method, target, protocol /*, content_type */;

    explicit Request(Socket s)
        : client(std::move(s))
    {}
    bool handler(void);
    void log_request(char* buffer);
    bool lexer(char* buffer);

private:
    Socket client;
};

#endif
