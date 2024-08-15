#include "tcp_client.h"
#include "hmi_test_data.h"


int main(int argc, char* argv[]) {

    if(argc<2)
    {
        std::cout<<"Usage: ./client <server_ip> <server_port>"<<std::endl;
        return 1;
    }

    {
        std::cout << "INFO_HEAD_E:" <<sizeof(INFO_HEAD_E)<<std::endl;
        std::cout << "ApaParkOutDir:" <<sizeof(ApaParkOutDir)<<std::endl;
        std::cout << "Data_head_interaction:" <<sizeof(Data_head_interaction)<<std::endl;
        std::cout << "Hmi_test_info:" <<sizeof(Hmi_test_info)<<std::endl;
        std::cout << "ApaAvapSlotOut:" <<sizeof(ApaAvapSlotOut)<<std::endl;
        std::cout << "ApaAvapObjOut:" <<sizeof(ApaAvapObjOut)<<std::endl;
        std::cout << "Apa_to_top_info:" <<sizeof(Apa_to_top_info)<<std::endl;

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