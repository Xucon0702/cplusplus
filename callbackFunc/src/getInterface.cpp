#include "getInterface.h"
#include <cstdio>

void CGetInterface::getNum1(int a)
{
    static int num = 0;
    printf("getnum1:%d,a=%d\n",num,a);
    num++;
}