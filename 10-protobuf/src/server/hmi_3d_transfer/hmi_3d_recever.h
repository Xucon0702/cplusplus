#ifndef HMI_3D_RECEVER_H_
#define HMI_3D_RECEVER_H_

/*server hmi 3d transfer interface*/

#include "data/hmi_3d_info.h"

class CHmi3DReceiver {
public:
    CHmi3DReceiver();
    ~CHmi3DReceiver();

    int32_t CheckData(PB_Hmi3dPackage* pb_hmi_3d_package,uint32_t dataType);
    int32_t RecvHmi3dPackage(int sock, PB_Hmi3dPackage* pb_hmi_3d_package);
    int32_t ReceiveUssPdc(int sock, PB_UssSectorOutputData* pb_uss_pdc);

    PB_Hmi3dPackage m_PB_Hmi3dPackage;
private:
    void PrintPB(const PB_UssSectorOutputData* pb_uss_pdc);
    void PrintPbHmi3dPackage(const PB_Hmi3dPackage& package);

    
private:
    char buffer[MAX_HMI_3D_INFO_LEN];

    PB_BaseHead* p_recved_head;
};










#endif