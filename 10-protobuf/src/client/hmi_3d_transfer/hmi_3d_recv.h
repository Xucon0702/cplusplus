#ifndef HMI_3D_RECE_H_
#define HMI_3D_RECE_H_

/*hmi 3d recv interface*/

#include "data/hmi_3d_info.h"

class CRecvHmi3D {
public:
    CRecvHmi3D();
    ~CRecvHmi3D();

    int32_t RecvHmi3dOutput(int sock, PB_Hmi3dOutputData* pb_hmi_3d_output);

public:
    PB_Hmi3dOutputData m_PB_Hmi3dOutputData;

private:
    void PrintHmi3DOutput(const PB_Hmi3dOutputData* pb_hmi_3d_output);
    void ConvertToHmiTestInfo(const PB_Hmi3dOutputData* pb_hmi_3d_output,Hmi_test_info* pDest);
    
private:
    char buffer[MAX_HMI_3D_INFO_LEN];

    Hmi_test_info m_Hmi_test_info;
};










#endif