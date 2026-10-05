#ifndef RESPONSE_H
#define RESPONSE_H

#include "net.h"
#include "request.h"
#include <map>
#include <string>

class Response {
public:
    Response( Network net, Request req,
              std::map< std::string, std::string > route_targets );

private:
    std::string content;
    std::string payload;
    std::string content_type = "text/html";
    int http_status = 200;

    std::string get_content( std::string target );
    std::string get_header( size_t content_size );
    std::string get_error_content( );

    std::map< int, std::string > http_messages = {
        { 100, "100 Continue" },
        { 200, "200 OK" },
        { 300, "300 Multiple Choices" },
        { 308, "308 Permanent Redirect" },
        { 400, "400 Bad Request" },
        { 404, "404 Not Found" },
        { 405, "405 Method Not Allowed" },
        { 408, "408 Request Timeout" },
        { 411, "411 Length Required" },
        { 413, "413 Content Too Large" },
        { 414, "414 URI Too Long" },
        { 415, "415 Unsupported Media Type" },
        { 429, "429 Too Many Requests" },
        { 431, "431 Request Header Fields Too Large" },
        { 500, "500 Internal Server Error" },
        { 501, "501 Not Implemented" },
        { 502, "502 Bad Gateway" },
        { 503, "503 Service Unavailable" },
        { 504, "504 Gateway Timeout" },
        { 505, "505 HTTP Version Not Supported" },
    };
};

#endif
