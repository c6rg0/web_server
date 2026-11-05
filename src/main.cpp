#include "server.h"
#include "request.h"
#include "response.h"
#include <signal.h>
#include <thread>
#include <vector>

void sig_handler(__attribute__((unused)) int sig)
{
    _exit(0);
}

int main(void)
{
    signal(SIGINT, sig_handler);

    Server server;

    const unsigned n = std::max(1u, std::thread::hardware_concurrency());
    std::vector<std::thread> threads;
    threads.reserve(n);

    for (unsigned i = 0; i < n; ++i) {
        threads.emplace_back(&Server::worker, &server);
    }

    for (auto& t : threads)
        t.join();

    return 0;
}
