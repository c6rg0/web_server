#include "net.h"
#include "request.h"
#include "response.h"
#include <string>
#include <map>
#include <signal.h>

// TODO: is it fine to have a global variable like this?
Network net;

void sig_handler(int sig)
{
    net.close();
    _exit(0);
}

int main(void)
{
    signal(SIGINT, sig_handler); 

    // <HTTP target, path target>
    std::map<std::string, std::string> route_targets = {
        { "/", "index.html" },
        { "/index", "index.html" },
    };

	while (1) {
        net.listen();
        Request req;
        net.read(req.buffer);
        req.lexer();
        Response res(net, req, route_targets);
	}

	return 0;
}
