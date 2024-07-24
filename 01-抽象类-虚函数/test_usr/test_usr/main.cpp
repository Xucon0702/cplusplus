#include <iostream>

using namespace std;
#include "ctesttransfer.h"
#include "lib/ctestlib.h"
int main()
{
    cout << "Hello World!" << endl;
    CBaseTransfer * pTran = new CTestTransfer();
    if(nullptr == pTran)
    {
        printf("error\n");
        fflush(stdout);
        return  0;
    }
    //CTestLib tLib1;
    CTestLib *tLib;
    tLib = nullptr;

    tLib = CTestLib::Init(pTran);
    if(nullptr != tLib)
    {
        tLib->Test1();
    }
    int t = 2;
    pTran->SendInt(&t);
    fflush(stdout);
    return 0;
}
