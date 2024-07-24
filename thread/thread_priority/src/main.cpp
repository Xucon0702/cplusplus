#include "thread_tools.h"
#include <iostream>

using namespace std;

static CThreadTools g_thread_tools;

void *thread_func1(void *arg)
{
    while(1)
    {
        // printf("thread_func1 runing\n");
        std::cout<<"thread_func1 runing\n"<<std::endl;
        usleep(1000*1000);        
    }
    
    return NULL;
}

void *thread_func2(void *arg)
{
    while(1)
    {
        // printf("thread_func1 runing\n");
        std::cout<<"thread_func2 runing\n"<<std::endl;
        usleep(1000*1000);        
    }
    
    return NULL;
}

int main()
{
    int ret = 0;
    int policy ,inher;

    // 初始化线程属性对象,设置一个线程的default值
    pthread_attr_t attr,attr2;
    ret = pthread_attr_init(&attr);
    if(ret != 0)
    {
        printf("attr_init error\n");
        return -1;
    }
    ret = pthread_attr_init(&attr2);
    if(ret != 0)
    {
        printf("attr_init error 2\n");
        return -1;
    }

    //显示各策略的最大/小优先级值
    g_thread_tools.ShowValidPriority();

    //设置线程名
    g_thread_tools.ShowThreadName(pthread_self());
    g_thread_tools.SetThreadName(pthread_self(),"thread_test");
    g_thread_tools.ShowThreadName(pthread_self());

    //显示当前线程的调度策略和优先级
    g_thread_tools.ShowSchedpolicy(pthread_self());

    // 设置线程的调度策略
    g_thread_tools.SetSchedpolicy(&attr,SCHED_RR);
    g_thread_tools.SetSchedpolicy(&attr2,SCHED_FIFO);
    
    // 设置线程的优先级
    g_thread_tools.SetPriority(&attr,5);
    g_thread_tools.SetPriority(&attr2,6);

    //设置继承性，不开启无法生效
	g_thread_tools.SetInher(true,&attr);
    g_thread_tools.SetInher(true,&attr2);
    
    //创建线程
    pthread_t thread01,thread02;
    // pthread_create(&thread, NULL, thread_func1, NULL);  //不设置线程属性

    if(ret = pthread_create(&thread01, &attr, thread_func1, NULL))
    {
        printf("fail to pthread_create:%s\n",strerror(ret));
        return -1;
    }

    g_thread_tools.ShowSchedpolicy(thread01);

    //方案2
    if(ret = pthread_create(&thread02, &attr2, thread_func2, NULL))
    {
        printf("fail to pthread_create2:%s\n",strerror(ret));
        return -1;
    }
    
    ret = pthread_setschedprio(thread02,8);//线程优先级设置
    if(ret)
        printf("fail to pthread_setschedprio:%s\n",strerror(ret));
    
    g_thread_tools.ShowSchedpolicy(thread02);
    
    while(true)
    {
        std::cout<<"main running\n"<<std::endl;
        usleep(1000*1000);
    }

    // 销毁线程属性对象
    pthread_attr_destroy(&attr);

    // 等待线程结束
    pthread_join(thread01, NULL);
    return 0;
}
