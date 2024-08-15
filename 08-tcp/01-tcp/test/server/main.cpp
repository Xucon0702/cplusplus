// #include "tcp_server.h"
#include "mv_hmi_sever_test.h"

int main() {
    // TCPServer server(8080);
    CMvHmiServerTest server(8080);
    server.start();

    return 0;
}