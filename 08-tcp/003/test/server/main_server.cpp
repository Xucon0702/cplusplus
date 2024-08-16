#include<cstring>
#include <unistd.h>
#include "service.h"

int main()
{	

    XTcp tcp,tmp_accept;
	int nReadNum=0;
    tcp.CreateSocket();
    tcp.Bind(SERVER_PORT);
    tcp.SetRecvTimeout(1);
    tcp.SetSendTimeout(1);
    tcp.Listen(MAX_CLINET_NUM);
    // tcp.SetBlock(true);

	// tmp_accept =tcp.Accept();
	char pbuf[1024];
	// string buf;
    while(1)
	{
		tmp_accept =tcp.Accept();
		// printf("main runing\n");

		memset(pbuf,0,1024);
		nReadNum = tcp.Recv(pbuf,4);
		if(nReadNum <=0)
		{
			printf("recv error\n");
			continue;
		}else
		{
			printf("recv:%d\n",*pbuf);
		}	
		
		usleep(100*1000);
		// int len = sizeof(caddr);
		
		// if(c<0)
		// {
		// 	continue;
        //     usleep(1000*1000);
		// }
 
		// char buf[128]={0};
		// int n = recv(c,buf,127,0);
		// printf("buf(%d)=%s\n",n,buf);
		// send(c,"ok",2,0);
 
		// close(c);//待机
    }

    return 0;
}
