#ifndef CTESTTRANSFER_H
#define CTESTTRANSFER_H

#include "lib/interface_base.h"
class CTestTransfer:public CBaseTransfer
{
public:
    CTestTransfer();
    virtual ~CTestTransfer() override;

    virtual void SendInt(int* pData) override;
    virtual void SendFloat(float* pData) override;
    virtual void GetInt(int* pData) override;


    void Sendto1();
};

#endif // CTESTTRANSFER_H
