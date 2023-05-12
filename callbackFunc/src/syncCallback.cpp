#include <stdio.h>
#include "typedef_callback.h"
 #include "getInterface.h"
//-----------------------底层实现A-----------------------------

void fCallBack(int a)
{

    printf("fCallBack:%d\n",a);
}

 
//留给应用层B的接口函数
void SetCallBackFun(int a, pcb callback)
{
    printf("A:start\n");
    callback(a);
    printf("A:end\n");
}

void callbackTest()
{
    SetCallBackFun(4,fCallBack);
}

 CRecvInterface::~CRecvInterface()
 {


 }

CGetInterface gCGetInterface;
void CRecvInterface::getNum(int a)
{
    gCGetInterface.getNum1(a);
}
