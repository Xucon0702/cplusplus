#include <cstring>
#include "tcp_server.h"

TCPServer::TCPServer(unsigned short port) : _listen_sock(0), _port(port) {}

void TCPServer::start() {
    if (_listen_sock != 0) {
        std::cerr << "Server already started." << std::endl;
        return;
    }

    _listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_listen_sock == -1) {
        perror("Error creating socket");
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(_listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        close(_listen_sock);
        return;
    }

    if (listen(_listen_sock, 5) == -1) {
        perror("Error listening on socket");
        close(_listen_sock);
        return;
    }

    std::cout << "Server started on port " << _port << std::endl;

    while (true) {
        accept_connections();
    }
}

void TCPServer::accept_connections() {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(_listen_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock == -1) {
        perror("Error accepting connection");
        return;
    }

    std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

    // Handle the client in a separate thread.
    // std::thread client_thread(&TCPServer::handle_client, this, client_sock);
    std::thread client_thread(&TCPServer::handle_client, this, client_sock);
    client_thread.detach();
}

// void TCPServer::handle_client(int client_sock) {
//     char buffer[256];
//     ssize_t bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
//     if (bytes_received > 0) {
//         buffer[bytes_received] = 'm';
//         buffer[bytes_received+1] = 'v';
//         buffer[bytes_received+2] = '\0';
//         std::cout << "Received: " << buffer << std::endl;
//         // send(client_sock, "Echo: " + std::string(buffer), strlen(buffer) + 7, 0);
//         send(client_sock,(void*)buffer, strlen(buffer) + 3, 0);//7
//     } else {
//         perror("Error receiving data");
//     }

//     close(client_sock);
// }