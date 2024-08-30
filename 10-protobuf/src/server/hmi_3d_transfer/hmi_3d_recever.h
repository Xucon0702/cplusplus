#ifndef HMI_3D_RECEVER_H_
#define HMI_3D_RECEVER_H_

/*server hmi 3d transfer interface*/

#include "data/hmi_3d_info.h"

class CHmi3DReceiver {
public:
    CHmi3DReceiver();
    ~CHmi3DReceiver();

    int32_t ReceiveUssPdc(int sock, PB_UssSectorOutputData* pb_uss_pdc);
private:
    void PrintPB(const PB_UssSectorOutputData* pb_uss_pdc);


private:
    char buffer[MAX_HMI_3D_INFO_LEN];
};










#endif