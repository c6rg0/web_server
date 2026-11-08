#ifndef REQUEST_H
#define REQUEST_H

#include "server.h"
#include <string>

class Request {
public:
    // TODO: Should the below be a map?
    std::string method, target, protocol /*, content_type */;
    char buffer [8192];

    explicit Request(Socket s)
        : client(std::move(s))
    {}
    bool handler();
    void log_request(void);
    bool lexer(void);

private:
    Socket client;
};

#endif
