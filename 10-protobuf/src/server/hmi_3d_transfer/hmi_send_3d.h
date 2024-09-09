#ifndef C_HMI_3D_SEND_H_
#define C_HMI_3D_SEND_H_

#include "data/hmi_3d_info.h"

//模拟界面的3d数据发送

#include <iostream>
#include <string.h>

class CHmiSend3DInf
{
public:
    CHmiSend3DInf();
    ~CHmiSend3DInf();

    int32_t SendHmi3dOutput(int sock,const Hmi_test_info& tHmi3dTestInfo);
    uint64_t GetCurrentTimestampMs();
private:
    
    void AddPbBaseHead(uint32_t uHead,uint32_t uDataType,PB_Hmi3dOutputData* pb_hmi_3d_output);
    int32_t ConvertHmi3dOutputPB(const Hmi_test_info& tHmi3dTestInfo, PB_Hmi3dOutputData* pb_hmi_3d_output);
private:
    //用于数据整包
    uint32_t m_sendPackageId;
    PB_Hmi3dOutputData m_PB_Hmi3dOutputData;
    PB_BaseHead* p_base_head;
};


#endif