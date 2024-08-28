#include <cstring>
#include <netinet/tcp.h>
#include "tcp_server.h"

TCPServer::TCPServer(uint16_t port,uint32_t nMaxClient, uint32_t nMaxBufNum,Thread_create_set tThread_create_set) : _listen_sock(0), _port(port),_nMaxClient(nMaxClient),_nMaxBufNum(nMaxBufNum),\
    _Thread_create_set(tThread_create_set)
{
    m_exitFlag = 0;

    memset(&m_NetQueueHandle,0,sizeof(NetQueueHandle));
    m_NetQueueHandle.nMaxClient = _nMaxClient;
    m_NetQueueHandle.nMaxBufNum = _nMaxBufNum;

    // m_handle_multiples_client_switch =  SUPORT_RECV_MULTIPLE_CLIENTS_SWTICH;
    // m_send_multiples_client_switch = SUPORT_SEND_MULTIPLE_CLIENTS_SWTICH;
    m_handle_multiples_client_switch =  _Thread_create_set.suport_multiple_client_recv;
    m_send_multiples_client_switch = _Thread_create_set.suport_multiple_client_send;

    std::thread listen_thread(&TCPServer::handle_listen,this);
    listen_thread.detach();

    printf("nMaxClient %d,nMaxBufNum %d\n",m_NetQueueHandle.nMaxClient,m_NetQueueHandle.nMaxBufNum);
}

void TCPServer::handle_listen()
{
    printf("start_listen\n");
    start();
}

void TCPServer::start() {
    if (_listen_sock != 0) {
        std::cerr << "Server already started." << std::endl;
        return;
    }

    _listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_listen_sock == -1) {
        perror("Error creating socket");
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //setsockopt
    int nSnBuf = 4*1024 * 1024;
    struct timeval tTimeOut = {0, 1000000};
	socklen_t nSocketLen = 0;
    int nKeepalive = 1; 
    int nNoDelay = 1;
    int opt = REUSER_IP_SWITCH;

    setsockopt(_listen_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    //以下设置可打开
    // setsockopt(_listen_sock, SOL_SOCKET, SO_SNDTIMEO, &tTimeOut, sizeof(tTimeOut));	
	// setsockopt(_listen_sock, SOL_SOCKET, SO_SNDBUF, &nSnBuf, sizeof(nSnBuf));	
	// getsockopt(_listen_sock, SOL_SOCKET, SO_SNDBUF, &nSnBuf, &nSocketLen);
    // setsockopt(_listen_sock, SOL_SOCKET, SO_KEEPALIVE, (void *)&nKeepalive, sizeof(nKeepalive));
    // setsockopt(_listen_sock, IPPROTO_TCP, TCP_NODELAY, (void *)&nNoDelay , sizeof(nNoDelay));


    if (bind(_listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        close(_listen_sock);
        return;
    }

    if (listen(_listen_sock, 5) == -1) {
        perror("Error listening on socket");
        close(_listen_sock);
        return;
    }

    std::cout << "Server started on port " << _port << std::endl;

    //开启接收和发送线程
    // pthread_create(&m_NetQueueHandle.tThreadRecvID, NULL, recv_thread, (void *)&m_NetQueueHandle);
    // pthread_create(&m_NetQueueHandle.tThreadSendID, NULL, send_thread, (void *)&m_NetQueueHandle);

    //这里需要在调用start()函数的线程去做退出
    while (true) {
        accept_connections();
        usleep(200*100);
    }
}

int TCPServer::checkCreatRecvHandle(void)
{
    int ret = 0;
    if(_Thread_create_set.creat_recv_handle_req == 0)
    {
        return 0;
    }

    if(m_handle_multiples_client_switch)
    {
        ret = 1;
    }
    else
    {
        if(!m_NetQueueHandle.aExistHandleInfo.b_recv_data_thread)
        {
            ret = 1;
        }
    }

	return ret;
}

int TCPServer::checkCreatSendHandle(void)
{
    int ret = 0;
    if(_Thread_create_set.creat_send_handle_req == 0)
    {
        return 0;
    }

    if(m_send_multiples_client_switch)
    {
        ret = 1;
    }
    else
    {
        if(!m_NetQueueHandle.aExistHandleInfo.b_send_data_thread)
        {
            ret = 1;
        }
    }

	return ret;
}

void TCPServer::accept_connections() {
    int nClientNo = 0;
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(_listen_sock, (struct sockaddr *)&client_addr, &client_len); //阻塞
    if (client_sock == -1) {
        perror("Error accepting connection");
        return;
    }

    // std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

    printf("accept_connections 1\n");
    //
    if(client_sock > 0)
    {
        std::lock_guard<std::mutex> lk(m_mutex_NetQueueHandle);
        if (m_NetQueueHandle.nMaxClient == m_NetQueueHandle.nClientConnectNum)
        {	
            printf("Now closed before all socketfd!\n");
            for (nClientNo = 0; nClientNo < m_NetQueueHandle.nMaxClient; nClientNo++)
            {
                close(m_NetQueueHandle.aClientFd[nClientNo].client_fd);
                m_NetQueueHandle.aClientFd[nClientNo].client_fd = 0;
                m_NetQueueHandle.aClientFd[nClientNo].b_creat_recv_handle = 0;	
                m_NetQueueHandle.aClientFd[nClientNo].b_creat_send_handle = 0;
                m_NetQueueHandle.nClientConnectNum--;

                //关闭之前创建的接收线程,保留待优化
                // if(m_NetQueueHandle.aClientFd[nClientNo].recv_handle_id)
                // {
                //     pthread_cancel(m_NetQueueHandle.aClientFd[nClientNo].recv_handle_id);
                //     pthread_kill(m_NetQueueHandle.aClientFd[nClientNo].recv_handle_id, 0);
                // }
                // if(m_NetQueueHandle.aClientFd[nClientNo].send_handle_id)
                // {
                //     pthread_cancel(m_NetQueueHandle.aClientFd[nClientNo].send_handle_id);
                //     pthread_kill(m_NetQueueHandle.aClientFd[nClientNo].send_handle_id, 0);
                // }
                // m_NetQueueHandle.aClientFd[nClientNo].recv_handle_id = 0;
                // m_NetQueueHandle.aClientFd[nClientNo].send_handle_id = 0;
            }
            //用于判断是否支持多个客户端链接处理
            m_NetQueueHandle.aExistHandleInfo.b_recv_data_thread = 0;
            m_NetQueueHandle.aExistHandleInfo.b_send_data_thread = 0;
            m_NetQueueHandle.aExistHandleInfo.recv_handle_id = 0;
            m_NetQueueHandle.aExistHandleInfo.send_handle_id = 0;
        }

        
        // if(checkCreatRecvHandle())
        {
            for (nClientNo = 0; nClientNo < m_NetQueueHandle.nMaxClient; nClientNo++)
            {				
                if (m_NetQueueHandle.aClientFd[nClientNo].client_fd <= 0)
                {					
                    m_NetQueueHandle.aClientFd[nClientNo].client_fd = client_sock;
                    // nAcceptConnfd = 0;	
                    m_NetQueueHandle.nClientConnectNum++;			

                    if(checkCreatRecvHandle())
                    {
                        //创建接收处理线程:同时接收多个客户端数据需要打开SUPORT_RECV_MULTIPLE_CLIENTS_SWTICH
                        std::thread client_thread(&TCPServer::handle_client, this, client_sock);  //接收
                        client_thread.detach();

                        //记录创建的线程信息
                        m_NetQueueHandle.aClientFd[nClientNo].b_creat_recv_handle = 1;
                        m_NetQueueHandle.aClientFd[nClientNo].recv_handle_id = *(unsigned int*)&(client_thread.get_id());  //结果为0,待优化
                        std::cout<<"client_thread.get_id()"<<client_thread.get_id()<<std::endl;

                        m_NetQueueHandle.aExistHandleInfo.b_recv_data_thread = 1;
                        m_NetQueueHandle.aExistHandleInfo.recv_handle_id = m_NetQueueHandle.aClientFd[nClientNo].recv_handle_id;

                    }
                    
                    if(checkCreatSendHandle())
                    {
                        //创建发送处理线程:
                        std::thread client_send_thread(&TCPServer::send_to_client, this, client_sock);  //接收
                        client_send_thread.detach();
                                            
                        m_NetQueueHandle.aClientFd[nClientNo].b_creat_send_handle = 1;
                        m_NetQueueHandle.aClientFd[nClientNo].send_handle_id = *(unsigned int*)&(client_send_thread.get_id());  //结果为0,待优化
                        std::cout<<"client_send_thread.get_id()"<<client_send_thread.get_id()<<std::endl;

                        m_NetQueueHandle.aExistHandleInfo.b_send_data_thread = 1;
                        m_NetQueueHandle.aExistHandleInfo.send_handle_id = m_NetQueueHandle.aClientFd[nClientNo].send_handle_id;
                    }
                                        
                    printf("Now connect num %d, allow %s to connect server!;recv_handle_id %d\n", m_NetQueueHandle.nClientConnectNum, inet_ntoa(client_addr.sin_addr),m_NetQueueHandle.aClientFd[nClientNo].recv_handle_id);

                    break;						
                }				
            }
        }
    }

    printf("accept_connections end\n");
}

// int TCPServer::MvGetSendTotalNum(void)
// {
// 	return m_NetQueueHandle.nSendTotalNum;
// }
void TCPServer::setExitFlag(uint8_t flag)
{
    static uint8_t tmp_flag = 0;
    if(flag != tmp_flag)
    {
        printf("setExitFlag %d\n",flag);
        tmp_flag = flag;
    }
    
    m_exitFlag = flag;
}
uint8_t TCPServer::getExitFlag()
{
    if(m_exitFlag)
    {
        printf("getExitFlag 1\n");
    }

    return m_exitFlag;
}

int32_t TCPServer::start_send(int32_t nFd, uint8_t *pData, uint32_t nLen)
{
	int32_t nRes = 0;
	int32_t nSendLen = 0;

    if(nFd <= 0)
    {
        return 0;
    }
    //  printf("start_send pData[0]=0x%x,pData[1]=0x%x\n",pData[0],pData[1]);
				
	while (nLen - nRes)
	{						
		nSendLen = send(nFd, pData + nRes, nLen - nRes, 0);
		if (nSendLen <= 0)
		{	
			close(nFd);
			return -1;					
		}		
		nRes += nSendLen;
	}			
	return 0;
}