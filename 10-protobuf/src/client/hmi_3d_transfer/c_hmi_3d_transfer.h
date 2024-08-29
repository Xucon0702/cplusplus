#ifndef C_HMI_3D_TRANSFER_H_
#define C_HMI_3D_TRANSFER_H_

/*client hmi 3d transfer interface*/

#include "data/hmi_3d_info.h"

class CCHmi_3d_transfer{
public:
    CCHmi_3d_transfer();
    ~CCHmi_3d_transfer();

private:
    /*protobuf data*/
    PB_UssSectorPdcData m_pdc_data;
};














#endif