//kill.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
 
void kill_usr_close(int pid);
void kill_usr_open(int pid);
 


int main(int argc)
{
    int a;

    scanf("%d", &a);
    kill_usr_close(a);
    sleep(5);
    kill_usr_open(a);
    return 0;
}
void kill_usr_close(int pid)
{
    kill(pid, SIGUSR1);
}
void kill_usr_open(int pid)
{
    kill(pid, SIGUSR2);
}