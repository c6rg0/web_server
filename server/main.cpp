#include "net.h"
#include "request.h"
#include "response.h"
#include <signal.h>

void sig_handler(__attribute__((unused)) int sig)
{
    _exit(0);
}

int main(void)
{
    signal(SIGINT, sig_handler);

    Server server;
    server.run();

    return 0;
}
