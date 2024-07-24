
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include "alg_test.h"


// typedef void (*TopicCallbackFun)(INT32 nTopicId, INT32 nCmdid, INT32 nSize,
// char *pData);

typedef int (*CallbackFun)(int32_t nTopicId, char *pData);  //注册用

uint32_t gCnt = 0;

// dds那些数据回调是有专门的线程做判断触发的

int Callback1()  // Callback Function 1
{
  printf("Hello, this is Callback_1 \n");
  return 0;
}

int Callback2()  // Callback Function 2
{
  printf("Hello, this is Callback_2 \n");
  return 0;
}

int Callback3()  // Callback Function 3
{
  printf("Hello, this is Callback_3 \n");
  int nA = 0;
  return 0;
}

int CallbackInt1(int tmp)  // Callback Function 3
{
  tmp = gCnt;
  printf("Hello, this is Callback_int_1;tmp = %d\n", tmp);
  return 0;
}

int Handle(int (*Callback)()) {
  printf("Entering Handle Function. \n");
  Callback();
  printf("Leaving Handle Function. \n");
  return 0;
}

//带参数的
int HandleInt(int (*Callback)(int)) {
  printf("Entering Handle Function. \n");
  Callback(999);
  printf("Leaving Handle Function. \n");
  return 0;
}

// Main program
int main() {
  // TODO(xucong)

  printf("Entering Main Function. \n");
  Handle(Callback1);
  Handle(Callback2);
  Handle(Callback3);
  printf("Leaving Main Function. \n");

  while (true) {
    sleep(1);
    // printf("runing\n");
    // HandleInt(Callback_int_1);

    alg_test();

    gCnt++;
  }

  return 0;
}