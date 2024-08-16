#include <cstring>
#include <iostream>
#include "mv_hmi_sever_test.h"


CMvHmiServerTest::CMvHmiServerTest(uint16_t port,uint32_t nMaxClient, uint32_t nMaxBufNum) : TCPServer(port,nMaxClient,nMaxBufNum) {}

#if 0
void CMvHmiServerTest::handle_client(int client_sock) {

    while(1)
    {
        printf("handle_client runing\n");

        usleep(50*1000);
    }
}
#else
void CMvHmiServerTest::handle_client(int client_sock) {

    while(!getExitFlag())
    {
        ssize_t bytes_received = recv(client_sock, &m_data_head, sizeof(Data_head_interaction), 0);

        // std::cout <<"m_data_head:"<<"CRC_HEAD:"<<m_data_head.crc_head<<";info_head:"<<m_data_head.info_head<<";payload_length:"<<m_data_head.payload_length<<std::endl;
        printf("bytes_received:%d;m_data_head:crc_head 0X%x;info_head 0x%x;payload_length %d\n",bytes_received,m_data_head.crc_head,m_data_head.info_head,m_data_head.payload_length);
        if (bytes_received > 0) {
            //判断是否是正确的数据
            if(m_data_head.crc_head == CRC_HEAD)
            {
                switch (m_data_head.info_head)
                {
                case HMI_OUT_INFO:
                    handle_hmi_test_data(client_sock,m_data_head.payload_length);
                    break;
                
                default:
                    std::cout << "Received: wrong info_head"<< std::endl;
                    break;
                }
            }
            else
            {
                std::cout << "Received: wrong crc_head"<< std::endl;
            }

        } else {
            perror("Error receiving data");
        }

        usleep(50*1000);
    }
    close(client_sock);
}
#endif

void CMvHmiServerTest::handle_hmi_test_data(int client_sock,uint32_t payload_length)
{
    //数据长度不对丢包
    if(payload_length != sizeof(Hmi_test_info))
    {
        printf("Error receiving Hmi_test_info data:payload_length %d != sizeof(Hmi_test_info) %d\n",payload_length,sizeof(Hmi_test_info));
        return ;
    }
    //数据处理
    std::cout << "handle_hmi_test_data"<< std::endl;
    memset(&m_hmi_test_info,0,sizeof(Hmi_test_info));
    ssize_t bytes_received = recv(client_sock, &m_hmi_test_info, sizeof(Hmi_test_info), 0); 
    if (bytes_received == sizeof(Hmi_test_info)) {
        #if 1
        printf("m_hmi_test_info:lTimestamp_ms %ld\n",m_hmi_test_info.lTimestamp_ms);
        printf("m_hmi_test_info:Hmi_apa_sel %d\n",m_hmi_test_info.Hmi_apa_sel);
        printf("m_hmi_test_info:Hmi_park_mode %d\n",m_hmi_test_info.Hmi_park_mode);
        printf("m_hmi_test_info:Hmi_start_park %d\n",m_hmi_test_info.Hmi_start_park);
        printf("m_hmi_test_info:Hmi_cancel_req %d\n",m_hmi_test_info.Hmi_cancel_req);
        printf("m_hmi_test_info:Hmi_pause_req %d\n",m_hmi_test_info.Hmi_pause_req);
        printf("m_hmi_test_info:Hmi_resum_req %d\n",m_hmi_test_info.Hmi_resum_req);
        printf("m_hmi_test_info:Hmi_tragParkConfSwt %d\n",m_hmi_test_info.Hmi_tragParkConfSwt);
        printf("m_hmi_test_info:Hmi_parkslot_id %d\n",m_hmi_test_info.Hmi_parkslot_id);
        #endif
        //冰凌转发

        //验证退出逻辑
        // if(m_hmi_test_info.lTimestamp_ms > 20)
        // {
        //     setExitFlag(1);
        // }
    }
    else {
        printf("Error receiving Hmi_test_info data:bytes_received %d != sizeof(Hmi_test_info) %d\n",bytes_received,sizeof(Hmi_test_info));
    }
}