#include "tcp_client.h"

TCPClient::TCPClient(const std::string& host, unsigned short port) : _sock(0), _host(host), _port(port) {}

bool TCPClient::connect_to_server() {
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_sock == -1) {
        perror("Error creating socket");
        return false;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    if (inet_pton(AF_INET, _host.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Failed to parse address: " << _host << std::endl;
        perror("Invalid address/ Address not supported");
        close(_sock);
        return false;
    }

    if (connect(_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(_sock);
        return false;
    }

    std::cout << "Connected to " << _host << ":" << _port << std::endl;
    return true;
}

void TCPClient::send_message(const std::string& message) {
    if (_sock == -1) {
        std::cerr << "Not connected." << std::endl;
        return;
    }

    if (send(_sock, message.c_str(), message.size(), 0) == -1) {
        perror("Error sending data");
    }
}

std::string TCPClient::receive_message() {
    if (_sock == -1) {
        std::cerr << "Not connected." << std::endl;
        return "";
    }

    char buffer[256];
    ssize_t bytes_received = recv(_sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        return std::string(buffer);
    } else {
        perror("Error receiving data");
        return "";
    }
}

TCPClient::~TCPClient() {
    if (_sock != -1) {
        close(_sock);
    }
}