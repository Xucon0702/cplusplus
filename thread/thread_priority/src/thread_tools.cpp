#include <iostream>
#include "thread_tools.h"

#include <sys/resource.h>

using namespace std;

CThreadTools::CThreadTools()
{

}

CThreadTools::~CThreadTools()
{

}

//显示线程名
void CThreadTools::ShowThreadName(pthread_t iThreadId)
{
    char thread_name[NAMELEN] = {0};
    //获取线程名
    memset(thread_name,0,NAMELEN);
    pthread_getname_np(iThreadId,thread_name,NAMELEN);
    printf("thread_name:%s\n",thread_name);

}
//设置线程名
int CThreadTools::SetThreadName(pthread_t iThreadId,char* threadName)
{
    return pthread_setname_np(iThreadId,threadName);
}

//显示各策略的优先级取值范围
void CThreadTools::ShowValidPriority()
{
    priority_t fifo_priority;
    priority_t rr_priority;
    priority_t other_priority;

    //获取系统可设置的各策略优先级的最大/最小值;SCHED_FIFO、SCHED_RR都是1~99
    fifo_priority.priority_max = sched_get_priority_max(SCHED_FIFO);
    fifo_priority.priority_min = sched_get_priority_min(SCHED_FIFO);
    cout<<"fifo_priority:"<<"Max:"<<fifo_priority.priority_max<<"Min:"<<fifo_priority.priority_min<<endl;

    rr_priority.priority_max = sched_get_priority_max(SCHED_RR);
    rr_priority.priority_min = sched_get_priority_min(SCHED_RR);
    cout<<"rr_priority:"<<"Max:"<<rr_priority.priority_max<<"Min:"<<rr_priority.priority_min<<endl;

    other_priority.priority_max = sched_get_priority_max(SCHED_OTHER );
    other_priority.priority_min = sched_get_priority_min(SCHED_OTHER );
    cout<<"other_priority:"<<"Max:"<<other_priority.priority_max<<"Min:"<<other_priority.priority_min<<endl;
}

/*
@brief:显示继承性
@return: 1 need set inheritance; 0 not need
*/
int CThreadTools::ShowInher(pthread_attr_t* attr,int &inher)
{
    int ret,funcRet=0;

    //获取继承的调度策略   
    ret = pthread_attr_getinheritsched(attr,&inher);  
    if (ret!=0)  
    {  
        printf("pthread_attr_getinheritsched failed\n%s\n", strerror(ret));  
        // exit(1); 
        funcRet = 0; 
    } 
    else
    {
        //   
        if (inher == PTHREAD_EXPLICIT_SCHED)        //不继承父进程调度策略以及优先级,设置才能修改优先级和调度策略
        {  
            printf("PTHREAD_EXPLICIT_SCHED\n");
            funcRet = 0;  
        }  
        else if (inher == PTHREAD_INHERIT_SCHED)    //隐式,默认继承
        {     
            printf("PTHREAD_INHERIT_SCHED\n");  
            inher = PTHREAD_EXPLICIT_SCHED;  
            funcRet = 1;
        }  

    }

    return funcRet;
}

/*
@brief:设置继承属性
@param：0 not set,just show;1 set EXPLICIT
*/
int CThreadTools::SetInher(bool bSet,pthread_attr_t* attr)
{
    int policy,inher,ret;

    if(ShowInher(attr,inher) && bSet)
    {
        //设置继承的调度策略   
        //必需设置inher的属性为 PTHREAD_EXPLICIT_SCHED，否则设置线程的优先级会被忽略   
        ret = pthread_attr_setinheritsched(attr, inher);  
        if (ret!=0)  
        {  
            printf("pthread_attr_setinheritsched\n%s\n", strerror(ret));  
            // exit(1);  
        }
        else
        {
           ShowInher(attr,inher); 
        }       
    }


    return ret;
}

//设置调度策略
int CThreadTools::SetSchedpolicy(pthread_attr_t* attr,int policy)
{
    // policy = SCHED_RR; //需要超级用户权限
	// pthread_attr_setschedpolicy( &attr, policy );//设置 调度策略为RR

    //如果设置attr导致pthread_create失败的话尝试设置限制
    // g_thread_tools.SetLimit();

    return pthread_attr_setschedpolicy(attr,policy);
}

//显示当前线程的调度策略和优先级
int CThreadTools::ShowSchedpolicy(pthread_t iThreadId)
{
    int policy, ret;  
    struct sched_param param;

    //获取调度策略和优先级
    ret = pthread_getschedparam(iThreadId,&policy, &param);  
    if(ret!=0){  
        printf("pthread_getschedparam %s\n", strerror(ret) );  
        return ret;  
    }  

    if (policy == SCHED_FIFO){  
        printf("policy:SCHED_FIFO\n");  
    }  
    else if (policy == SCHED_OTHER){  
        printf("policy:SCHED_OTHER\n");  
    }  
    else if (policy == SCHED_RR){  
        printf("policy:SCHED_RR\n");  
    }  
    printf("thread %d priority is %d\n",iThreadId,param.sched_priority); 

    return ret;
}


int CThreadTools::SetLimit()
{
    struct rlimit limit;
    int err = 0;
 
    memset(&limit, 0, sizeof(limit));
    limit.rlim_cur = RLIM_INFINITY; //软限制，表示对资源没有限制
    limit.rlim_max = RLIM_INFINITY; //硬限制，这个参数表示对资源没有限制，一定要大于等于rlim_cur值
    if(err = setrlimit(RLIMIT_RTPRIO, &limit)) //RLIMIT_CORE
    {
        printf("fail to setrlimit:%s\n",strerror(err));
    }

    return 0;
}

//设置优先级
int CThreadTools::SetPriority(pthread_attr_t* attr,int priority)
{
    struct sched_param param;
    param.sched_priority = priority;
    
    return pthread_attr_setschedparam(attr, &param);
}