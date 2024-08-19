#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class TCPClient {
public:
    TCPClient(const std::string& host, unsigned short port);
    void init();
    bool connect_to_server();
    void send_message(const std::string& message);
    
    void send_message(const void* message, size_t size);
    std::string receive_message();

    ~TCPClient();

protected:
    
    int _sock;
private:
    
    std::string _host;
    unsigned short _port;
};

#endif // TCP_CLIENT_H