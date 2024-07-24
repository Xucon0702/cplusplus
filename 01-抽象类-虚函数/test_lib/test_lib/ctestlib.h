#ifndef CTESTLIB_H
#define CTESTLIB_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(TEST_LIB_LIBRARY)
#  define TEST_LIB_EXPORT Q_DECL_EXPORT
#else
#  define TEST_LIB_EXPORT Q_DECL_IMPORT
#endif
//软件调用库内的接口
#include "interface_base.h"
class TEST_LIB_EXPORT CTestLib
{
private:
    CTestLib();
public:
    ~CTestLib();
    static CTestLib* Init(CBaseTransfer *pSendPtr);
    CBaseTransfer *GetTransfer();
    void Test1();
    void TestAlgInit();
    void Test1AlgNew();
private:
    CBaseTransfer *m_SendPtr;
};

#endif // CTESTLIB_H
