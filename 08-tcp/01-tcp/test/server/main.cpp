// #include "tcp_server.h"
#include "mv_hmi_sever_test.h"

int main() {

    {
        std::cout << "INFO_HEAD_E:" <<sizeof(INFO_HEAD_E)<<std::endl;
        std::cout << "ApaParkOutDir:" <<sizeof(ApaParkOutDir)<<std::endl;
        std::cout << "Data_head_interaction:" <<sizeof(Data_head_interaction)<<std::endl;
        std::cout << "Hmi_test_info:" <<sizeof(Hmi_test_info)<<std::endl;
        std::cout << "ApaAvapSlotOut:" <<sizeof(ApaAvapSlotOut)<<std::endl;
        std::cout << "ApaAvapObjOut:" <<sizeof(ApaAvapObjOut)<<std::endl;
        std::cout << "Apa_to_top_info:" <<sizeof(Apa_to_top_info)<<std::endl;

    }
    // TCPServer server(8080);
    CMvHmiServerTest server(8080);
    // CMvHmiServerTest server(8080,100,99);
    server.start();

    return 0;
}