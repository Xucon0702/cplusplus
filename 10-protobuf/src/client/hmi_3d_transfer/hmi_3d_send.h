#ifndef HMI_3D_SEND_H_
#define HMI_3D_SEND_H_

/*client hmi 3d transfer interface*/

#include "data/hmi_3d_info.h"
// #include "hmi_3d/mv_uss.pb.h"

class CHmi3DSendInf
{
public:
    CHmi3DSendInf();
    ~CHmi3DSendInf();


    int32_t SendData(int sock,void *data,uint32_t size);
    // int32_t SendUssPdc(int sock, const ZU2UssSectorInfo_t& uss_pdc);
    int32_t SendUssPdc(int sock, const ZU2UssSectorOutputData_t& uss_pdc);

    int32_t ConvertUssPdcToPB(const ZU2UssSectorOutputData_t& uss_pdc, PB_UssSectorOutputData* pb_uss_pdc);
private:
    

private:
    /*protobuf data*/
    PB_UssSector_FrameHeader* p_frame_header;
    PB_UssSectorInfo* p_UssSectorInfo;
    // PB_UssSectorOutputData m_pdc_data;

    PB_UssSectorOutputData pb_uss_pdc;

};














#endif