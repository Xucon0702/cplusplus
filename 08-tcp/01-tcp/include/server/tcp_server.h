#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex> 

#define SUPORT_RECV_MULTIPLE_CLIENTS_SWTICH  0 //是否支持接收多个客户端数据做处理:1-支持;0-不支持,只接收处理第一个接入的客户端数据;不影响发送

#define REUSER_IP_SWITCH  1
#define MAX_CLIENT_NUM  5   //初始化时不能超过这个数
#define MAX_BUF_NUM  4

typedef struct{
    uint8_t b_recv_data_thread;//是否已有处理接收数据的线程
    pthread_t recv_handle_id;  //接收处理线程的id
}Recv_handle_info;

typedef struct ClientLinkInfo_
{
    uint8_t b_creat_recv_handle; //是否已创建接收的处理线程
    pthread_t recv_handle_id;   //创建的接收处理线程;待优化
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
    Recv_handle_info aRecvHandleInfo;
} NetQueueHandle;

class TCPServer {
public:
    TCPServer(uint16_t port,uint32_t nMaxClient, uint32_t nMaxBufNum);
    void start();
    
protected:
    virtual void handle_client(int client_sock) = 0;

private:
    void accept_connections();
    int checkCreatRecvHandle(void);

    int _listen_sock;
    uint16_t _port;
    uint32_t _nMaxClient;
    uint32_t _nMaxBufNum;
    uint32_t m_handle_multiples_client_swtich;

    std::mutex m_mutex_NetQueueHandle;
    NetQueueHandle m_NetQueueHandle;
};

#endif // TCP_SERVER_H