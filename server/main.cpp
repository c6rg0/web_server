#include "net.h"
#include "request.h"
#include "response.h"
#include <map>
#include <signal.h>
#include <string>

Network net;

void sig_handler(__attribute__((unused)) int sig)
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

        int err = net.read(req.buffer);
        if (err != 0) {
            req.log_request();
            continue;
        }

        req.lexer();
        Response res(net, req, route_targets);
    }

    return 0;
}
