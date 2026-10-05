#include "net.h"
#include "request.h"
#include "response.h"
#include <string>
#include <map>

int main(void)
{
    // <HTTP target, path target>
    std::map<std::string, std::string> route_targets = {
        { "/", "index.html" },
        { "/index", "index.html" },
    };

	// TODO: Add signal termination
    Network net;

	while (1) {
        net.listen();

        Request req;
        net.read(req.buffer);
        req.lexer();

        Response res(net, req, route_targets);
	}

    net.close();

	return 0;
}
