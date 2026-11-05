#include "request.h"
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

bool Request::handler()
{
    ssize_t request = ::read(client.fd(), buffer, 8192);

    if (request <= 0) {
        std::cerr << "(Request contains no data or read error)" << std::endl;
        return true;
    }

    // std::cerr << buffer << std::endl;

    log_request();
    bool corrupt = lexer();
    return corrupt;
}

void Request::log_request(void)
{
    std::filesystem::path file_path = "/tmp/c6rg0_web_server";
    std::filesystem::create_directory(file_path);

    if (!std::filesystem::exists(file_path)) {
        std::cerr << "Error: Couldn't create debug directory!" << std::endl;
        exit(1);
    }

    std::time_t time = std::time(nullptr);
    std::string file_name = std::to_string(time) + ".http";
    file_path = file_path / file_name;
    std::ofstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file!" << std::endl;
        exit(1);
    }

    file << buffer;
    file.close();
}

bool Request::lexer(void)
{
    char* token = std::strtok(buffer, " \n\r\t");
    if (!token) {
        std::cerr << "Lexer couldn't get method" << std::endl;
        return true;
    }
    method = token;

    token = std::strtok(nullptr, " \n\r\t");
    if (!token) {
        std::cerr << "Lexer couldn't get target" << std::endl;
        return true;
    }
    target = token;

    token = std::strtok(nullptr, " \n\r\t");
    if (!token) {
        std::cerr << "Lexer couldn't get protocol" << std::endl;
        return true;
    }
    protocol = token;

    return false;
}
