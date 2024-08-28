#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex> 
#include <atomic>

#define SUPORT_RECV_MULTIPLE_CLIENTS_SWTICH  0 //是否支持接收多个客户端数据做处理:1-支持;0-不支持,只接收处理第一个接入的客户端数据;不影响发送
#define SUPORT_SEND_MULTIPLE_CLIENTS_SWTICH  1 //是否支持发送多个客户端数据:1-支持;0-不支持,默认支持多客户端发送

#define REUSER_IP_SWITCH  1
#define MAX_CLIENT_NUM  5   //初始化时不能超过这个数
#define MAX_BUF_NUM  4

typedef struct 
{
    uint8_t creat_recv_handle_req;          //创建接收线程请求:1-需要创建
    uint8_t creat_send_handle_req;          //创建发送线程请求:1-需要创建
    uint8_t suport_multiple_client_recv;
    uint8_t suport_multiple_client_send;
    /* data */
}Thread_create_set;



typedef struct{
    uint8_t b_recv_data_thread;//是否已有处理接收数据的线程
    uint8_t b_send_data_thread;//是否已有处理接收数据的线程
    pthread_t recv_handle_id;  //接收处理线程的id
    pthread_t send_handle_id;   //发送处理线程的id
}Exist_handle_info;

typedef struct ClientLinkInfo_
{
    uint8_t b_creat_recv_handle; //是否已创建接收的处理线程
    uint8_t b_creat_send_handle; //是否已创建发送的处理线程
    pthread_t recv_handle_id;   //创建的接收处理线程;待优化
    pthread_t send_handle_id;   //创建的发送处理线程;待优化
    int client_fd;
}ClientLinkInfo;

typedef struct 
{	
	uint32_t nNetPort;
	uint32_t nMaxClient;  //最大连接数
	uint32_t nMaxBufNum;
	int nSockFd;
	uint32_t nClientConnectNum;
	ClientLinkInfo aClientFd[MAX_CLIENT_NUM];
	uint32_t nSendTotalNum;
	uint32_t nResetFlag;   //发送重置缓存标志
	pthread_t tThreadRecvID;
	pthread_t tThreadSendID;
    Exist_handle_info aExistHandleInfo;
} NetQueueHandle;

class TCPServer {
public:
    TCPServer(uint16_t port,uint32_t nMaxClient, uint32_t nMaxBufNum,Thread_create_set tThread_create_set);
    void handle_listen();
    void start();  //需要单独开启线程运行start

    void setExitFlag(uint8_t flag);
    uint8_t getExitFlag();
    int32_t start_send(int32_t nFd, uint8_t *pData, uint32_t nLen);
    
protected:
    virtual void handle_client(int client_sock) = 0; //接收处理线程
    virtual void send_to_client(int client_sock) = 0;//发送处理线程

    std::mutex m_mutex_NetQueueHandle;
    NetQueueHandle m_NetQueueHandle;

private:
    void accept_connections();
    int checkCreatRecvHandle(void);
    int checkCreatSendHandle(void);

    Thread_create_set _Thread_create_set;
    std::atomic<uint8_t> m_exitFlag;

    int _listen_sock;
    uint16_t _port;
    uint32_t _nMaxClient;
    uint32_t _nMaxBufNum;
    uint32_t m_handle_multiples_client_switch;//接收
    uint32_t m_send_multiples_client_switch;//发送

    // std::mutex m_mutex_NetQueueHandle;
    // NetQueueHandle m_NetQueueHandle;
};

#endif // TCP_SERVER_H