#include<cstring>
#include <unistd.h>
#include "service.h"

int main()
{	
	// while(1)
	// {
		XTcp tcp;
		if (tcp.Connect(5))
		{
			char buf[1024] = { 0 };
			while (1)
			{
				tcp.Recv(buf, sizeof(buf));
				printf("recv:%s\n", buf);
				tcp.Send(buf, strlen(buf));

				usleep(1000*1000);
			}
		}
		else
		{
			printf("connect error\n");
		}

	// }
    return 0;
}
