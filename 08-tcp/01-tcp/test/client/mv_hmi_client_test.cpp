#include <cstring>
#include <iostream>
#include <thread>
#include "mv_hmi_client_test.h"


CMvHmiClientTest::CMvHmiClientTest(const std::string& ip, int port):\
TCPClient(ip,port){
	std::thread client_recv_handle(&CMvHmiClientTest::recv_handle,this);
	client_recv_handle.detach();
}


CMvHmiClientTest::~CMvHmiClientTest()
{

}


void CMvHmiClientTest::recv_handle()
{
	std::cout << "recv data:_sock %d"<<_sock << std::endl;

	while(1)
	{
		if (_sock == -1) {
			std::cerr << "Not connected." << std::endl;
			usleep(2000*1000);
			continue;
    	}

		memset(&m_data_head,0,sizeof(Data_head_interaction));
		ssize_t bytes_received = recv(_sock, &m_data_head, sizeof(Data_head_interaction), 0);
		printf("sizeof(Data_head_interaction) %d\n",sizeof(Data_head_interaction));
		printf("bytes_received:%d;m_data_head:crc_head 0X%x;info_head 0x%x;payload_length %d\n",bytes_received,m_data_head.crc_head,m_data_head.info_head,m_data_head.payload_length);

		if (bytes_received > 0) {
            //判断是否是正确的数据
            if(m_data_head.crc_head == CRC_HEAD)
            {
                switch (m_data_head.info_head)
                {
                case TRANSPORT_PARK_INFO:
                    {
						recv_slot_info(_sock,m_data_head.payload_length);												
					}
                    break;

                case TRANSPORT_OD_INFO:
                    {
						recv_od_info(_sock,m_data_head.payload_length);
					}
                    break;
				
				case TRANSPORT_APA_STATE_INFO:
                    {
						recv_apastate_info(_sock,m_data_head.payload_length);
					}
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

		usleep(20*1000);
	}
}

int CMvHmiClientTest::recv_slot_info(int nSock,uint32_t payload_length)
{
	printf("Received: TRANSPORT_PARK_INFO:\n");

	//数据长度不对丢包
    if(payload_length != sizeof(ApaAvapSlotOut))
    {
        printf("Error receiving ApaAvapSlotOut data:payload_length %d != sizeof(ApaAvapSlotOut) %d\n",payload_length,sizeof(ApaAvapSlotOut));
        return -1;
    }

	ssize_t bytes_received = recv(_sock, &m_ApaAvapSlotOut, sizeof(ApaAvapSlotOut), 0);
	if(bytes_received > 0)
	{
		printf("m_ApaAvapSlotOut:TimeMsec %ld\n",m_ApaAvapSlotOut.TimeMsec);
	}

	return 0;
}

int CMvHmiClientTest::recv_od_info(int nSock,uint32_t payload_length)
{
	printf("Received: TRANSPORT_OD_INFO:\n");
	//数据长度不对丢包
    if(payload_length != sizeof(ApaAvapObjOut))
    {
        printf("Error receiving ApaAvapObjOut data:payload_length %d != sizeof(ApaAvapObjOut) %d\n",payload_length,sizeof(ApaAvapObjOut));
        return -1;
    }

	ssize_t bytes_received = recv(_sock, &m_ApaAvapObjOut, sizeof(ApaAvapObjOut), 0);
	if(bytes_received > 0)
	{
		printf("m_ApaAvapObjOut:TimeMsec %ld\n",m_ApaAvapObjOut.TimeMsec);
	}
	return 0;
}

int CMvHmiClientTest::recv_apastate_info(int nSock,uint32_t payload_length)
{
	printf("Received: TRANSPORT_APA_STATE_INFO:\n");

	//数据长度不对丢包
    if(payload_length != sizeof(Apa_to_top_info))
    {
        printf("Error receiving Apa_to_top_info data:payload_length %d != sizeof(Apa_to_top_info) %d\n",payload_length,sizeof(Apa_to_top_info));
        return -1;
    }

	ssize_t bytes_received = recv(_sock, &m_Apa_to_top_info, sizeof(Apa_to_top_info), 0);
	if(bytes_received > 0)
	{
		printf("m_Apa_to_top_info:lTimestampMs %ld\n",m_Apa_to_top_info.lTimestampMs);
	}

	return 0;
}