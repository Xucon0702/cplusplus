#include "ctestlib.h"
#include <stdio.h>
CTestLib::CTestLib()
{
    m_SendPtr = nullptr;
}

CTestLib::~CTestLib()
{

}

CTestLib *CTestLib::Init(CBaseTransfer *pSendPtr)
{
    if(nullptr != pSendPtr)
    {
        static CTestLib * tmp = new CTestLib();
        tmp->m_SendPtr = pSendPtr;
        return tmp;
    }
    return nullptr;
}

CBaseTransfer *CTestLib::GetTransfer()
{
    return m_SendPtr;
}

void CTestLib::Test1()
{
    int a,b;
    float c = 12.0;
    a = 10;
    b = 10;
    GetTransfer()->SendInt(&a);
    GetTransfer()->SendFloat(&c);
    GetTransfer()->GetInt(&b);
    printf("b = %d\n",b);
}
