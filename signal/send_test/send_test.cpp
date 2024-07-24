#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>

using namespace std;

int main(int argc,char* argv[])
{
    int pid = 0;
    int signalId = 0;

    if(argc<2)
    {
        cout<<"please input process id"<<endl;
        return 0;
    }    

    cout<<"please input signal id"<<endl;
    cout<<"1-SIGUSR1  2-SIGUSR2"<<endl;

    scanf("%d",&signalId);
    getchar();
    
    if(signalId<1 || signalId>2)
    {
        cout<<"signal id not suport:"<<signalId<<endl;
        return 0;
    }
        
    pid = atoi(argv[1]);

    switch (signalId)
    {
    case 1:
        kill(pid, SIGUSR1);
        break;

    case 2:
        kill(pid, SIGUSR2);
        break;

    default:
        break;
    }

    printf("pid = %d;signalId %d\n",pid,signalId);

    return 0;
}