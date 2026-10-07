#include "response.h"
#include "request.h"
#include "net.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>

Response::Response (Network net, Request req, std::map<std::string, std::string> route_targets)
{
    if ((req.method.compare("GET")) != 0) {
        http_status = 405;
        content = get_error_content();
    }

    else {
        std::string target = route_targets[req.target];
        content = get_content(target);
    }

    payload = get_header(content.size());
    payload.append(content);
    net.write(payload);
}

std::string Response::get_content (std::string target)
{
    std::filesystem::path file_path = std::filesystem::current_path();

    if (!target.empty()){
        file_path = file_path / target;

        if (!std::filesystem::exists(file_path)){
            std::cerr << "Error: Server-side defined file does not exist!\n";
            http_status = 500;
            content = get_error_content();
        }

        else {
            std::ifstream file (file_path);

            if (!file.is_open ()) {
                std::cerr << "Error: Unable to open file!\n";
                http_status = 500;
                content = get_error_content();
            }

            else {
                // TODO: Assuming that all content is text
                content.assign((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
            }

            file.close();
        }
    }

    else {
        http_status = 404;
        content = get_error_content();
    }

    return content;
}

std::string Response::get_header (size_t content_size)
{
    std::string http_response = http_messages[http_status];
    std::string content_size_str = std::to_string(content_size);

    std::string header =
        "HTTP/1.1 " 
        + http_response +
        "\n" 
        "Content-Type: " 
        + content_type +
        "\n" 
        "Content-Size: " 
        + content_size_str +
        "\n" 
        "Connection: keep-alive\n"
        "Keep-Alive: timeout=5\n"
        "\n";

    return header;
}

std::string Response::get_error_content ()
{
    std::string http_response = http_messages[http_status];

    content_type = "application/json";
    content = 
        "{\"error\": \"" 
        + http_response + 
        "\"}";

    return content;
}
