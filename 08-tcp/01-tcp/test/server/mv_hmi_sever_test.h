#ifndef MV_HMI_SERVER_TEST_H
#define MV_HMI_SERVER_TEST_H
#include <memory>
#include "tcp_server.h"
#include "hmi_test_data.h"

#define	MAX_QUEUE_BUF_NUM	16

typedef struct
{	
	uint32_t nReadIdx;
	uint32_t nSendIdx;
	uint32_t nWriteIdx;
    uint32_t aBuf[MAX_QUEUE_BUF_NUM];
	// MvNetBuf aBuf[MAX_QUEUE_BUF_NUM];
} NetSendQueue;





class CMvHmiServerTest:public TCPServer{
public:
    CMvHmiServerTest(uint16_t port,uint32_t nMaxClient = MAX_CLIENT_NUM, uint32_t nMaxBufNum = MAX_BUF_NUM,Thread_create_set tThread_create_set = m_thread_create_set);

    void  sendData(uint8_t *payload,uint32_t payload_length,uint8_t info_type);
protected:
    void handle_client(int client_sock) override;//重写接收数据的处理
    void send_to_client(int client_sock) override;//重写发送数据的处理
private:
    void handle_hmi_test_data(int client_sock,uint32_t payload_length);

    void create_head(uint8_t *pHeadData, uint8_t nCmdId, uint32_t nLen);
    void sendApaAvapSlotOut(int client_fd,uint8_t *payload,uint32_t payload_length,uint8_t info_type);
    void sendApaAvapObjOut(int client_fd,uint8_t *payload,uint32_t payload_length,uint8_t info_type);
    void sendApa_to_top_packet(int client_fd,uint8_t *payload,uint32_t payload_length,uint8_t info_type);
    
private:
    static const Thread_create_set m_thread_create_set;
    
    //recv
    Data_head_interaction m_data_head;
    // Hmi_test_info m_hmi_test_info;  
    std::shared_ptr<Hmi_test_info> m_hmi_test_info = nullptr;


    //send
};















#endif