#ifndef INTERFACE_BASE_H
#define INTERFACE_BASE_H

class CBaseTransfer
{
public:
    virtual void SendInt(int* pData)=0;
    virtual void SendFloat(float* pData)=0;
    virtual void GetInt(int* pData)=0;
    virtual ~CBaseTransfer(){};
protected:
    CBaseTransfer(){}
};
#endif // INTERFACE_BASE_H
