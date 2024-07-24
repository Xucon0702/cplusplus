#ifndef INTERFACE_BASE_H
#define INTERFACE_BASE_H
//算法库内调用软件层的接口
class CBaseTransfer
{
public:
    virtual void SendInt(int* pData)=0;
    virtual void SendFloat(float* pData)=0;//发送float数据
    virtual void GetInt(int* pData)=0;//需要给pData赋值
    virtual ~CBaseTransfer(){};
protected:
    CBaseTransfer(){}
};
#endif // INTERFACE_BASE_H
