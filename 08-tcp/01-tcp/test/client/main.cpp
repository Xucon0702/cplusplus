
#include <cstring>
#include <thread>
// #include "tcp_client.h"
#include "mv_hmi_client_test.h"
#include "hmi_test_data.h"




void send_error_info(TCPClient& client) //乱数据做验证
{
    char buf[10] = "hel";
    client.send_message(buf,10);
}
void send_hmi_info(TCPClient& client)
{
    Hmi_test_packet t_Hmi_test_packet;
    static uint8_t tmp_cnt=0;

    memset(&t_Hmi_test_packet,0,sizeof(Hmi_test_packet));

    t_Hmi_test_packet.tHead.crc_head = CRC_HEAD;
    t_Hmi_test_packet.tHead.info_head = INFO_HEAD_E::HMI_OUT_INFO;//
    t_Hmi_test_packet.tHead.payload_length = sizeof(Hmi_test_info);
    
    t_Hmi_test_packet.tData.lTimestamp_ms = (uint64_t)tmp_cnt+1;
    t_Hmi_test_packet.tData.Hmi_apa_sel = (uint8_t)tmp_cnt+2;
    t_Hmi_test_packet.tData.Hmi_park_mode = (uint8_t)tmp_cnt+3;
    t_Hmi_test_packet.tData.Hmi_start_park = (uint8_t)tmp_cnt+4;
    t_Hmi_test_packet.tData.Hmi_cancel_req = (uint8_t)tmp_cnt+5;
    t_Hmi_test_packet.tData.Hmi_Selected = (uint8_t)tmp_cnt+6;
    t_Hmi_test_packet.tData.Hmi_pause_req = (uint8_t)tmp_cnt+7;
    t_Hmi_test_packet.tData.Hmi_resum_req = (uint8_t)tmp_cnt+8;
    t_Hmi_test_packet.tData.Hmi_tragParkConfSwt = (uint8_t)tmp_cnt+9;
    t_Hmi_test_packet.tData.Hmi_parkslot_id = (uint32_t)tmp_cnt+10;

    tmp_cnt++;

    client.send_message(&t_Hmi_test_packet,sizeof(Hmi_test_packet));

    

    #if 1
    printf("t_Hmi_test_packet:crc_head:0x%x\n",t_Hmi_test_packet.tHead.crc_head);
    printf("t_Hmi_test_packet:info_head:0x%x\n",t_Hmi_test_packet.tHead.info_head);
    printf("t_Hmi_test_packet:payload_length:%d\n",t_Hmi_test_packet.tHead.payload_length);

    printf("t_Hmi_test_packet:lTimestamp_ms:%ld\n",t_Hmi_test_packet.tData.lTimestamp_ms);
    printf("t_Hmi_test_packet:Hmi_apa_sel:%d\n",t_Hmi_test_packet.tData.Hmi_apa_sel);
    printf("t_Hmi_test_packet:Hmi_park_mode:%d\n",t_Hmi_test_packet.tData.Hmi_park_mode);
    printf("t_Hmi_test_packet:Hmi_start_park:%d\n",t_Hmi_test_packet.tData.Hmi_start_park);
    printf("t_Hmi_test_packet:Hmi_cancel_req:%d\n",t_Hmi_test_packet.tData.Hmi_cancel_req);
    printf("t_Hmi_test_packet:Hmi_Selected:%d\n",t_Hmi_test_packet.tData.Hmi_Selected);
    printf("t_Hmi_test_packet:Hmi_pause_req:%d\n",t_Hmi_test_packet.tData.Hmi_pause_req);
    printf("t_Hmi_test_packet:Hmi_resum_req:%d\n",t_Hmi_test_packet.tData.Hmi_resum_req);
    printf("t_Hmi_test_packet:Hmi_tragParkConfSwt:%d\n",t_Hmi_test_packet.tData.Hmi_tragParkConfSwt);
    printf("t_Hmi_test_packet:Hmi_parkslot_id:%d\n",t_Hmi_test_packet.tData.Hmi_parkslot_id);
    #endif
}

// void recv_handle(int socket)
// {
//     while(1)
//     {
//         client.OnRecv();
//         usleep(20*1000);
//     }
// }

int main(int argc, char* argv[]) {

    if(argc<2)
    {
        std::cout<<"Usage: ./client <server_ip> <server_port>"<<std::endl;
        return 1;
    }

    {
        std::cout << "INFO_HEAD_E:" <<sizeof(INFO_HEAD_E)<<std::endl;
        std::cout << "ApaParkOutDir:" <<sizeof(ApaParkOutDir)<<std::endl;
        // std::cout << "Data_head_interaction:" <<sizeof(Data_head_interaction)<<std::endl;
        printf("sizeof(Hmi_test_packet) %d;sizeof(Data_head_interaction)\n",sizeof(Hmi_test_packet),sizeof(Data_head_interaction));
        std::cout << "Hmi_test_info:" <<sizeof(Hmi_test_info)<<std::endl;
        std::cout << "ApaAvapSlotOut:" <<sizeof(ApaAvapSlotOut)<<std::endl;
        std::cout << "ApaAvapObjOut:" <<sizeof(ApaAvapObjOut)<<std::endl;
        std::cout << "Apa_to_top_info:" <<sizeof(Apa_to_top_info)<<std::endl;

    }

    const std::string& host_ip = argv[1];
    const int port = std::stoi(argv[2]);    
        
    uint32_t num = 0;

    // TCPClient client(host_ip, port);
    CMvHmiClientTest client(host_ip, port);

    if (client.connect_to_server()) {
            // client.send_message("Hello, Server!");  
    }
    else
    {
        return 1;
    }

    // std::thread client_recv_handle(recv_handle,client._sock);
    // std::thread client_recv_handle(&client.recv_handle,client._sock);
    
    
    while(1)
    {
        
        if(num%2 == 0)
        {
            send_hmi_info(client);    
        }
        else
        {
            send_error_info(client); 
        }
        usleep(100*1000);

        num++;
    }
    // client_recv_handle.detach();

    return 0;
}