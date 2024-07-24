#ifndef THREAD_TOOLS_H_
#define THREAD_TOOLS_H_

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

#include <string.h>

#define NAMELEN 64

typedef struct 
{
    int priority_max = 0;
    int priority_min = 0;
}priority_t;

class CThreadTools
{
public:
    CThreadTools();
    ~CThreadTools();

    void ShowThreadName(pthread_t iThreadId);
    int SetThreadName(pthread_t iThreadId,char* threadName);

    void ShowValidPriority();
    int ShowInher(pthread_attr_t* attr,int &inher);
    int SetInher(bool bSet,pthread_attr_t* attr);
    int SetSchedpolicy(pthread_attr_t* attr,int policy);
    int ShowSchedpolicy(pthread_t iThreadId);

    int SetLimit();
    int SetPriority(pthread_attr_t* attr,int priority);
    
private:
    pthread_attr_t attr;
};










#endif