#include<stdio.h>

#include<stdlib.h>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <thread>

#define DISALLOW_COPY_AND_ASSIGN(classname)                                    \
    classname(const classname &) = delete;                                     \
    classname &operator=(const classname &) = delete;
//单例
#define DECLARE_SINGLETON(classname)                                                \
public:                                                                                              \
    static classname *Instance()                                                               \
    {                                                                                                \                                                      
        static classname instance;                                                                   \
        return &instance;                                                                            \
    }\
private:                                                                                             \
    classname();                                                                                \
    DISALLOW_COPY_AND_ASSIGN(classname)

class CThreadProcess
{
    DECLARE_SINGLETON(CThreadProcess)
public:
    ~CThreadProcess();

    void Start();

    void Stop();

    void Run();

    void ThreadProc();

    void SetThreadProc(void (*pThreadProc)(void *), void *pParam);

    void SetThreadName(const char *pName);

    void SetThreadPriority(int nPriority);

    void WaitApaActive();
    void ResumeApa();
    void PauseApa();

private:
    std::mutex mtx_apa_active;
    // bool flag_apa_active = true;
    bool flag_apa_active;
    std::condition_variable cv_apa_active;
};

CThreadProcess::CThreadProcess()
{

}

CThreadProcess::~CThreadProcess()
{

}

void CThreadProcess::WaitApaActive()
{
    std::unique_lock <std::mutex> lock(mtx_apa_active);
    if(!flag_apa_active)
    {
        printf("WaitApaActive on\n");
    }
    cv_apa_active.wait(lock,[&]{return flag_apa_active;});
}

void CThreadProcess::ResumeApa()
{
    std::lock_guard <std::mutex> lock(mtx_apa_active);
    flag_apa_active = true;
    cv_apa_active.notify_all();
}

void CThreadProcess::PauseApa()
{
    std::lock_guard <std::mutex> lock(mtx_apa_active);
    flag_apa_active = false;
}


void thread1()
{
    while(1)
    {
        printf("thread1\n");
        CThreadProcess::Instance()->ResumeApa();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void thread2()
{
    while (1)
    {
        printf("thread2\n");
        CThreadProcess::Instance()->PauseApa();
        std::this_thread::sleep_for(std::chrono::milliseconds(80)); //单位毫秒
    }
    
}



int main()
{
    printf("main start\n");

    std::thread myThread1(thread1);
    std::thread myThread2(thread2);
    
    while(1)
    {
        CThreadProcess::Instance()->WaitApaActive();

        printf("main loop\n");
        usleep(80*1000);
    }

    myThread1.join();
    myThread2.join();
    
    return 0;
}