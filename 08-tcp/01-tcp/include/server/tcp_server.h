#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class TCPServer {
public:
    TCPServer(unsigned short port);
    void start();

protected:
    virtual void handle_client(int client_sock) = 0;

private:
    void accept_connections();
    // void handle_client(int client_sock);
    
    int _listen_sock;
    unsigned short _port;
};

#endif // TCP_SERVER_H