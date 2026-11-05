#include "response.h"
#include "net.h"
#include "request.h"
#include <cerrno>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <netdb.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

void Response::handler(std::string method, std::string target)
{
    if ((method.compare("GET")) != 0) {
        http_status = 405;
        content = get_error_content();
    }

    else {
        auto iterator = route_targets.find(target);

        if (iterator != route_targets.end()) {
            content = get_content(iterator->second);
        }

        else {
            http_status = 404;
            content = get_error_content();
        }
    }

    payload = get_header(content.size());
    payload.append(content);
    write(payload);
}

std::string Response::get_content(std::string file_name)
{
    std::filesystem::path file_path = std::filesystem::current_path();

    if (file_name.empty()) {
        http_status = 404;
        return get_error_content();
    }

    file_path = file_path / file_name;

    if (!std::filesystem::exists(file_path)) {
        std::cerr << "Error: Server-side defined file does not exist!"
                  << std::endl;
        http_status = 500;
        return get_error_content();
    }

    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file!" << std::endl;
        http_status = 500;
        return get_error_content();
    }

    // TODO: Add compatibility with binary data
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}

std::string Response::get_header(size_t content_size)
{
    std::string http_response = http_messages [http_status];
    std::string content_size_str = std::to_string(content_size);

    std::string header = "HTTP/1.1 " + http_response +
                         "\r\n"
                         "Content-Type: " +
                         content_type +
                         "\r\n"
                         "Content-Length: " +
                         content_size_str +
                         "\r\n"
                         "Connection: close\r\n"
                         "\r\n";

    return header;
}

std::string Response::get_error_content()
{
    std::string http_response = http_messages [http_status];

    content_type = "application/json";
    content = "{\"error\": \"" + http_response + "\"}";

    return content;
}

void Response::write(std::string payload)
{
    for (;;) {
        ssize_t response;
        response = ::write(client.fd(), payload.data(), payload.size());

        if (response < 0) {
            std::cerr << "(Data wasn't written to target)" << std::endl;
            if (errno == EINTR)
                continue;
            break;
        }

        if (response == 0) {
            break;
        }

        else {
            std::cerr << "(Data sent to target)" << std::endl;
            break;
        }
    }
}
