#ifndef REQUEST_H
#define REQUEST_H

#include "net.h"
#include <string>

class Request {
public:
    std::string method, target, protocol;
    //          content_type;
    char buffer [8192];

    explicit Request(Socket s)
        : client(std::move(s))
    {
    }
    void handler();
    void log_request(void);
    void lexer(void);

private:
    Socket client;
};

#endif
