// #include "tcp_server.h"
#include <cstring>
#include "mv_hmi_sever_test.h"
#include <signal.h>

// CMvHmiServerTest server(8080);
// CMvHmiServerTest server(8080,5,4);
// CMvHmiServerTest server(8080,5,4,thread_create_set);

void signal_pipe_func()
{
    printf("SIGPIPE catched\n");
}


CMvHmiServerTest gServer(8080,5,4);
void init_server ()
{
    // CMvHmiServerTest server(8080);
    // CMvHmiServerTest server(8080,5,4);
    // CMvHmiServerTest server(8080,5,4,thread_create_set);
    gServer.start();
}

void send_handle ()
{
    ApaAvapSlotOut tApaAvapSlotOut;
    ApaAvapObjOut tApaAvapObjOut;
    Apa_to_top_info tApa_to_top_info;

    memset(&tApaAvapSlotOut,0,sizeof(ApaAvapSlotOut));
    memset(&tApaAvapObjOut,0,sizeof(ApaAvapObjOut));
    memset(&tApa_to_top_info,0,sizeof(Apa_to_top_info));

    uint8_t num = 0;

    signal(SIGPIPE,signal_pipe_func); //捕获tcp send失败时发送的SIGPIPE导致程序退出的问题

    while(1)
    {
        tApaAvapSlotOut.TimeMsec = num+1;
        gServer.sendData((uint8_t*)&tApaAvapSlotOut,sizeof(ApaAvapSlotOut),TRANSPORT_PARK_INFO);
        usleep(100*1000);

        tApaAvapObjOut.TimeMsec = num+2;
        gServer.sendData((uint8_t*)&tApaAvapObjOut,sizeof(ApaAvapObjOut),TRANSPORT_OD_INFO);
        usleep(100*1000);

        tApa_to_top_info.lTimestampMs = num+3;
        gServer.sendData((uint8_t*)&tApa_to_top_info,sizeof(Apa_to_top_info),TRANSPORT_APA_STATE_INFO);

        usleep(200*1000);

        num++;
    }

}

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

    std::thread tcp_server(init_server);
    std::thread server_send(send_handle);

    // // CMvHmiServerTest server(8080);
    // CMvHmiServerTest server(8080,5,4);
    // // CMvHmiServerTest server(8080,5,4,thread_create_set);
    // server.start();

    while(1)
    {
        printf("man running\n");
        usleep(500*1000);
    }

    printf("man 111\n");
    tcp_server.detach();
    server_send.detach();
    printf("man end\n");
    return 0;
}