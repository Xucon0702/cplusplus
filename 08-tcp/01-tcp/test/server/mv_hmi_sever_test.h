#ifndef MV_HMI_SERVER_TEST_H
#define MV_HMI_SERVER_TEST_H
#include "tcp_server.h"
#include "hmi_test_data.h"

class CMvHmiServerTest:public TCPServer{
public:
    CMvHmiServerTest(unsigned short port);

protected:
    void handle_client(int client_sock) override;//重写数据处理
private:
    void handle_hmi_test_data(int client_sock,uint32_t payload_length);
private:
    Data_head_interaction m_data_head;
    Hmi_test_info m_hmi_test_info;  
};













#endif