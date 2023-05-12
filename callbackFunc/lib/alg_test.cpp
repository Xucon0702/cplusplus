#include <stdio.h>
#include "typedef_callback.h"
#include "alg_test.h"

static int tmp = 0;

static CRecvInterface  gCRecvInterface;

void alg_test()
{
    printf("alg_test start:tmp %d\n",tmp);
    tmp++;
    callbackTest();
    gCRecvInterface.getNum(tmp+1);
}