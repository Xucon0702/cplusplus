#include <iostream>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

void set_timer(uint32_t seconds) {
    struct itimerval timer;
    timer.it_value.tv_sec = seconds;
    timer.it_value.tv_usec = 0;
    timer.it_interval = timer.it_value;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("Error disabling timer");
        exit(EXIT_FAILURE);
    }
}

// 函数用于关闭定时器
void disable_timer() {
    struct itimerval timer;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval = timer.it_value;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("Error disabling timer");
        exit(EXIT_FAILURE);
    }
}

// 信号处理函数
void handle_alarm(int signum) {
    static int count = 0;
    cout << "Timer triggered " << ++count << " times." << endl;

    if(count > 10)
    {
        disable_timer();
    }
}



int main() {
    // 设置信号处理函数
    signal(SIGALRM, handle_alarm);

    // 定义定时器结构体
    // struct itimerval timer;

    // // 初始化定时器
    // timer.it_value.tv_sec = 1; // 1 second
    // timer.it_value.tv_usec = 0; // no microseconds
    // timer.it_interval = timer.it_value; // repeat every 1 second

    // // 设置定时器
    // if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
    //     perror("Error setting timer");
    //     return 1;
    // }

    // set_timer(1);

    // 主循环，让程序持续运行
    while (true) {
        // 这里可以做一些其他的工作
        // 由于信号处理是非阻塞的，因此定时器触发不会影响主循环
        sleep(1); // 让出 CPU 时间片，等待信号
    }

    return 0;
}