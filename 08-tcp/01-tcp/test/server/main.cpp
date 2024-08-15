#include "tcp_server.h"

int main() {
    TCPServer server(8080);
    server.start();

    return 0;
}