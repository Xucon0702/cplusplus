#include "ctesttransfer.h"
#include <stdio.h>
CTestTransfer::CTestTransfer()
{

}

CTestTransfer::~CTestTransfer()
{

}

void CTestTransfer::SendInt(int *pData)
{
    printf("SendInt = %d\n",*pData);
    //ipc->send(pData);
}

void CTestTransfer::SendFloat(float *pData)
{
    printf("SendFloat = %f\n",*pData);
}

void CTestTransfer::GetInt(int *pData)
{
    printf("GetInt = %d\n",*pData);
    *pData = 66;
    //syncget(pData);
}

void CTestTransfer::Sendto1()
{

}
