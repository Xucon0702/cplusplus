#include "mv_hmi_sever_test.h"
#include <cstring>
#include <iostream>


CMvHmiServerTest::CMvHmiServerTest(unsigned short port) : TCPServer(port) {}

void CMvHmiServerTest::handle_client(int client_sock) {
    char buffer[256];
    ssize_t bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Received: " << buffer << std::endl;
        send(client_sock, (void*)buffer, strlen(buffer) + 3, 0);
    } else {
        perror("Error receiving data");
    }

    close(client_sock);
}