#include "request.h"
#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <iostream>

Request::Request ()
{
    req_count++;
}

void Request::log_request (void)
{
    std::string file_path = "/tmp/request_" + std::to_string(req_count);

    std::ofstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file!\n";
        exit(1);
    }

    file << buffer;
    file.close();
    req_count++;
}

void Request::lexer (void)
{
    char* token = std::strtok(buffer, " \n\r\t");
    if (!token){
        std::cout << "broken\n";
        exit(1);
    }
    method = token;

    token = std::strtok(nullptr, " \n\r\t");
    if (!token){
        std::cout << "broken\n";
        exit(1);
    }
    target = token;

    token = std::strtok(nullptr, " \n\r\t");
    if (!token){
        std::cout << "broken\n";
        exit(1);
    }
    protocol = token;
}
