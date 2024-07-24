#include <stdio.h>
#include <thread>
#include <cstring>
#include <unistd.h>

float thread_stack_usage_cal()
{
    size_t rsp_value;
    asm volatile ("movq %%rsp, %0" : "=rm" (rsp_value));
    pthread_attr_t attr;
    void *stack_addr;
    size_t stack_size;
    memset(&attr, 0, sizeof(pthread_attr_t));
    pthread_getattr_np(pthread_self(), &attr);
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);  //获取线程的堆栈地址和大小
    // pthread_attr_destroy(&attr);

    auto avail = rsp_value - (size_t)stack_addr; //计算距离最高栈顶的距离为剩余栈空间
    auto used = stack_size - avail; //总空间-剩余空间=已使用的栈空间

    auto usage = (float)((float)used / (float)stack_size)*100;//计算使用率

    printf("thread stack info: used = %ld, avail = %ld, total = %ld  usage = %lf%%\n", 
        used,
        avail,
        stack_size,
        usage);
    return usage;
}


void test()
{
    static unsigned long stack_size = 1024*1024*1;
    while(1)
    {
        unsigned char test[stack_size];
        thread_stack_usage_cal();
        stack_size+=1024*1024*1;

        sleep(2);
    }

}

#define SIZE_ADD_ONCE  (1024*1)
void test2()
{
    static unsigned long stack_size = SIZE_ADD_ONCE;
    while(1)
    {
        unsigned char test[stack_size];
        thread_stack_usage_cal();
        stack_size+=SIZE_ADD_ONCE;

        sleep(1);
    }

}

int main()
{
    static unsigned long stack_size = 1024*1024*1;
    unsigned char testArray[stack_size];
    thread_stack_usage_cal();

    std::thread thread1(test);
    std::thread thread2(test2);
    thread1.join();
    thread2.join();
    return 0;
}
