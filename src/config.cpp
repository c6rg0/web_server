#include "config.h"

#include <map>
#include <string>

namespace config
{
/*
 * The TCP port that the server will use:
 */
int port = 8080;

/*
 * Use the below to declare what files are to be accessable:
 * { HTTP URL path, fs path relative to project root }
 */
std::map<std::string, std::string> route_targets = {
    { "/", "index.html" },
    { "/index", "index.html" },
};
}
