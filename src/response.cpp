#include "response.h"
#include "request.h"
#include "server.h"
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
    this->method = method;
    this->target = target;

    std::string content;
    // std::vector<int> content (?);

    if ((this->method.compare("GET")) != 0) {
        http_status = 405;
        content = get_error_content();
    }

    else {
        auto iterator = route_targets.find(this->target);

        if (iterator != route_targets.end()) {
            content = get_content(iterator->second);
        }

        else {
            http_status = 404;
            content = get_error_content();
        }
    }

    std::string http_response = http_messages [http_status];
    std::string content_size = std::to_string(content.size());

    std::string payload = "HTTP/1.1 " + http_response +
                          "\r\n"
                          "Content-Type: " +
                          content_type +
                          "\r\n"
                          "Content-Length: " +
                          content_size +
                          "\r\n"
                          "Connection: close\r\n"
                          "\r\n";

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

    std::string extension = std::filesystem::path(file_path).extension();
    // content_type = content_type_messages [extension];

    auto iterator = content_type_messages.find(extension);

    if (iterator != content_type_messages.end()) {
        content_type = iterator->second;
    }

    else {
        http_status = 415;
        return get_error_content();
    }

    // This supports non-text data
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    return stream.str();
}

std::string Response::get_error_content()
{
    std::string http_response = http_messages [http_status];
    content_type = "application/json";
    std::string content = "{\"error\": \"" + http_response + "\"}";

    return content;
}

void Response::write(std::string payload)
{
    for (;;) {
        ssize_t response;
        response = ::write(client.fd(), payload.data(), payload.size());

        if (response < 0) {
            std::cerr << "(Data wasn't written to target)" << std::endl;

            if (errno == EINTR) {
                http_status = 400;
                handler(this->method, this->target);
            }

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
