#ifndef XTCP_H
#define XTCP_H
 
#include <string>
 
#define SERVER_PORT 8010  //7000
#define MAX_CLINET_NUM 5//最大连接数量

class XTcp
{
public:
	int CreateSocket();				 //创建套接字
	bool Bind(unsigned short port);  //绑定端口号
	bool Listen(unsigned short num); //监听端口号
	bool SetBlock(bool isblock);  //设置阻塞模式  （希望只有在connect的时候是非阻塞的，而接收数据时候是阻塞的）
	bool Connect(const char *ip = "192.168.0.123", unsigned short port = 8000, int sec = 3);
	bool Connect(int sec = 3);
	XTcp Accept();                   //返回XTcp对象，接收连接
	void Close();							//关闭连接
	int Recv(char *buf, int size);          //接收数据
	int Send(const char *buf, int size);	//发送数据
	int SetRecvTimeout(int sec = 1);			 //设置udp接收超时
	int SetSendTimeout(int sec = 1);		     //设置udp发送超时
 
 
	char clientip[16];						//存放接收到的client ip
	unsigned short clientport = 0;			//存放接收到的client port
 
	XTcp(unsigned short port = 8000);
    XTcp(char *ip,unsigned short port);
	virtual ~XTcp();
 
 
private:
	char tcp_serverip[16] = "";   //tcp_serverip 服务端ip
	int tsock = 0;							//tcp客户端的socket，create自动生成
	unsigned short tport = 0;				//接收main函数中的参数  要传入Connect
};
 
#endif