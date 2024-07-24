//signal.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>

int m_switch = 1;
void sig_usr_close(int sig);
void sig_usr_open(int sig);
void sig_alrm(int sig);

#define TEST_STR    "hello world"

int main()
{
    if(signal(SIGUSR1, sig_usr_close) == SIG_ERR)
    {
        printf("signal error\n");
    }
    if(signal(SIGUSR2, sig_usr_open) == SIG_ERR)
    {
        printf("signal error\n");
    }

    if(signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        printf("signal error\n");
    }
    printf("%d\n", getpid());

    alarm(5);//定时器,以最后一个调用为准
    // alarm(10);//定时器
    // alarm(15);//定时器

    while(1)
    {
        if(m_switch == 1)
        {
            printf("hello:strlen(TEST_STR) = %d\n",strlen(TEST_STR));
        }
        sleep(1);
    }
    return 0;
}
void sig_usr_close(int sig)
{
    printf("reveive SIGUSER1\n");
    m_switch = 0;
}
void sig_usr_open(int sig)
{
    printf("reveive SIGUSER2\n");
    m_switch = 1;
}

void sig_alrm(int sig)
{
    printf(" sig_alrm recved:sig %d\n",sig);
}

