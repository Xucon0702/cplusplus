#ifndef TYPEDEF_CALLBACK_H_
#define TYPEDEF_CALLBACK_H_

typedef void (*pcb)(int a); //函数指针定义，后面可以直接使用pcb，方便
void SetCallBackFun(int a, pcb callback);

void callbackTest();

class CRecvInterface
{   
public:
    ~CRecvInterface();

    void getNum(int a);
    

};

#endif
