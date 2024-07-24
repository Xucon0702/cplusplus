#include <stdio.h>
#include "typedef_callback.h"
#include "getInterface.h"
//-----------------------底层实现A-----------------------------

void FCallBack(int a)
{

    printf("FCallBack:%d\n",a);
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
    SetCallBackFun(4,FCallBack);
}

 CRecvInterface::~CRecvInterface()
 {


 }

CGetInterface gCGetInterface;
void CRecvInterface::GetNum1(int a)
{
    gCGetInterface.GetNum1(a);
}
