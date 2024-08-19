#ifndef MV_HMI_CLIENT_TEST_H
#define MV_HMI_CLIENT_TEST_H
#include "tcp_client.h"
#include "hmi_test_data.h"

class CMvHmiClientTest:public TCPClient{
public:
    CMvHmiClientTest(const std::string& ip, int port);
    CMvHmiClientTest::~CMvHmiClientTest();
    
    void recv_handle();
    int recv_slot_info(int nSock,uint32_t payload_length);
    int recv_od_info(int nSock,uint32_t payload_length);
    int recv_apastate_info(int nSock,uint32_t payload_length);
protected:
    
private:
    //recv
    Data_head_interaction m_data_head;
    ApaAvapSlotOut m_ApaAvapSlotOut;
    ApaAvapObjOut m_ApaAvapObjOut;
    Apa_to_top_info m_Apa_to_top_info;
};







#endif