#include <cstring>
#include <iostream>
#include "mv_hmi_sever_test.h"

const Thread_create_set CMvHmiServerTest::m_thread_create_set = {1,1,0,1}; //结构体默认参,默认不支持多客户端接收
CMvHmiServerTest::CMvHmiServerTest(uint16_t port,uint32_t nMaxClient, uint32_t nMaxBufNum,Thread_create_set tThread_create_set) : \
TCPServer(port,nMaxClient,nMaxBufNum,tThread_create_set) {}

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
        printf("sizeof(Data_head_interaction) %d\n",sizeof(Data_head_interaction));
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

void CMvHmiServerTest::create_head(uint8_t *pHeadData, uint8_t nCmdId, uint32_t nLen)
{
	pHeadData[0] = (CRC_HEAD & 0xff);
	pHeadData[1] = ((CRC_HEAD >> 8) & 0xff);
	pHeadData[2] = nCmdId;
	pHeadData[3] = (nLen & 0xff);
	pHeadData[4] = ((nLen>>8) & 0xff);
	pHeadData[5] = ((nLen>>16) & 0xff);
	pHeadData[6] = ((nLen>>24) & 0xff);
}



/*******************************发送处理**********************************/
/*
1、单独开辟一个线程去发送数据，需要添加队列缓存做拷贝发送
2、直接在数据源头调用send接口发送
*/
void CMvHmiServerTest::send_to_client(int client_sock) {

    while(!getExitFlag())
    {


        usleep(50000*1000);
    }

    close(client_sock);
    printf("send_to_client thread exit:%d\n",client_sock);
}


void  CMvHmiServerTest::sendData(uint8_t *payload,uint32_t payload_length,uint8_t info_type)
{
    if(payload == NULL)
    {
        return;
    }

    int connectedNum = m_NetQueueHandle.nClientConnectNum>5?5:m_NetQueueHandle.nClientConnectNum;

    for (int nClientNo = 0; nClientNo < connectedNum; nClientNo++)
    {				
        if (m_NetQueueHandle.aClientFd[nClientNo].client_fd > 0)
        {
            //发送数据
            switch(info_type)
            {
                case TRANSPORT_PARK_INFO:
                    {
                        sendApaAvapSlotOut(m_NetQueueHandle.aClientFd[nClientNo].client_fd,payload,payload_length,info_type);
                    }
                    break;
                case TRANSPORT_OD_INFO:
                    {
                        sendApaAvapObjOut(m_NetQueueHandle.aClientFd[nClientNo].client_fd,payload,payload_length,info_type);
                    }
                    break;
                case TRANSPORT_APA_STATE_INFO:
                    {
                        sendApa_to_top_packet(m_NetQueueHandle.aClientFd[nClientNo].client_fd,payload,payload_length,info_type);
                    }
                    break;
                default:
                    {
                        printf("unknow info_type\n");
                    }
                        
                    break;
            }
            
            
        }

        //发送
    }
}

void CMvHmiServerTest::sendApaAvapSlotOut(int client_fd,uint8_t *payload,uint32_t payload_length,uint8_t info_type)
{
    if(payload == NULL)
    {
        return;
    }

    auto  p_packet = std::make_shared<ApaAvapSlotOut_packet>(); 
    memcpy(&p_packet->tData,payload,payload_length);   
    // memset(&p_packet,0,sizeof(ApaAvapSlotOut_packet));

    create_head((uint8_t *)&p_packet->tHead,info_type,payload_length);

    start_send(client_fd,(uint8_t *)p_packet.get(),sizeof(ApaAvapSlotOut_packet));

    uint8_t* buf = (uint8_t *)p_packet.get();

    printf("p_packet[0]=0x%x,p_packet[1]=0x%x\n",buf[0],buf[1]);
    printf("sendApaAvapSlotOut done:tHead crc_head 0x%x,info_head 0x%x,payload_length %d\n",p_packet->tHead.crc_head,p_packet->tHead.info_head,p_packet->tHead.payload_length);
}

void CMvHmiServerTest::sendApaAvapObjOut(int client_fd,uint8_t *payload,uint32_t payload_length,uint8_t info_type)
{
    if(payload == NULL)
    {
        return;
    }

    auto  p_packet = std::make_shared<ApaAvapObjOut_packet>();    
    memcpy(&p_packet->tData,payload,payload_length);
    
    create_head((uint8_t *)&p_packet->tHead,info_type,payload_length);

    start_send(client_fd,(uint8_t *)p_packet.get(),sizeof(ApaAvapObjOut_packet));

    printf("sendApaAvapObjOut done\n");
}

void CMvHmiServerTest::sendApa_to_top_packet(int client_fd,uint8_t *payload,uint32_t payload_length,uint8_t info_type)
{
    if(payload == NULL)
    {
        return;
    }

    auto  p_packet = std::make_shared<Apa_to_top_packet>();    
    memcpy(&p_packet->tData,payload,payload_length);

    create_head((uint8_t *)&p_packet->tHead,info_type,payload_length);

    start_send(client_fd,(uint8_t *)p_packet.get(),sizeof(Apa_to_top_packet));

    printf("sendApa_to_top_packet done\n");
}