#include "tcp_client.h"

int main(int argc, char* argv[]) {

    if(argc<2)
    {
        std::cout<<"Usage: ./client <server_ip> <server_port>"<<std::endl;
        return 1;
    }
    const std::string& host_ip = argv[1];
    const int port = std::stoi(argv[2]);    

    // TCPClient client("127.0.0.1", 8080);
    TCPClient client(host_ip, port);
    if (client.connect_to_server()) {
        client.send_message("Hello, Server!");
        std::string response = client.receive_message();
        std::cout << "Server responded: " << response << std::endl;
    }

    return 0;
}