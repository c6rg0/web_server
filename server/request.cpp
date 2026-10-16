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

void Request::handler()
{
    ssize_t request = ::read(client.fd(), buffer, 8192);

    if (request <= 0) {
        std::cerr << "(Request contains no data or read error)\n";
        exit(1);
    }

    log_request();
    lexer();
}

void Request::log_request(void)
{
    std::filesystem::path file_path = "/tmp/c6rg0_web_server";
    std::filesystem::create_directory(file_path);

    if (!std::filesystem::exists(file_path)) {
        std::cerr << "Error: Couldn't create debug directory!\n";
        exit(1);
    }

    std::time_t time = std::time(nullptr);
    std::string file_name = std::to_string(time) + ".http";
    file_path = file_path / file_name;
    std::ofstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file!\n";
        exit(1);
    }

    file << buffer;
    file.close();
}

void Request::lexer(void)
{
    char* token = std::strtok(buffer, " \n\r\t");
    if (!token) {
        std::cout << "broken\n";
        exit(1);
    }
    method = token;

    token = std::strtok(nullptr, " \n\r\t");
    if (!token) {
        std::cout << "broken\n";
        exit(1);
    }
    target = token;

    token = std::strtok(nullptr, " \n\r\t");
    if (!token) {
        std::cout << "broken\n";
        exit(1);
    }
    protocol = token;
}
