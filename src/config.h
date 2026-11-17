#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>

/*
 * Comment out the below macro if you don't want the server
 * to re-use addresses.
 */
#define reuse_address

/* 
 * Configure the rest in config.cpp,
 * leave the below intact.
 */
namespace config
{
extern int port;
extern std::unordered_map<std::string, std::string> route_targets;
}

#endif
