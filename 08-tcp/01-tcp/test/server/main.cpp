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
    Thread_create_set thread_create_set;
    thread_create_set.creat_recv_handle_req = 0;
    thread_create_set.creat_send_handle_req = 1;
    thread_create_set.suport_multiple_client_recv = 0;
    thread_create_set.suport_multiple_client_send = 1;

    // CMvHmiServerTest server(8080);
    CMvHmiServerTest server(8080,5,4);
    // CMvHmiServerTest server(8080,5,4,thread_create_set);
    server.start();

    return 0;
}