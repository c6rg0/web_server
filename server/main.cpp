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

    while (1) {
        net.listen();
        Request req;
        int err = net.read(req.buffer);

        if (err != 0) {
            req.log_request();
            continue;
        }

        req.lexer();

        Response res(req.method, req.target);

        /* TODO:
         * The below method will take in a Network obj, but if
         * it was to run concurrently in seperate Response
         * objects, it'd overwrite clientfd in Network.
         *
         * I need to seperate the sock init from the rest of
         * Network so that multiple clients can simultaneously
         * exist, thus make multithreading work.
         */
        res.handler(net);
    }

    return 0;
}
