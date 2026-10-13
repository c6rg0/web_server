#ifndef REQUEST_H
#define REQUEST_H

#include <string>

class Request {
public:
    std::string method, target, protocol;
    //          content_type;

    char buffer [8192];

    Request();
    void log_request(void);
    void lexer(void);

private:
    int req_count = 0;
};

#endif
