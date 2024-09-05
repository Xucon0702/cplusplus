#ifndef HMI_3D_SEND_H_
#define HMI_3D_SEND_H_

/*hmi 3d send interface*/

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
    // int32_t SendHmi3dPackage(int sock, const ZU2UssSectorOutputData_t& uss_pdc);
    int32_t SendHmi3dPackage(int sock);

    int CleanHmi3DQueue();
    int GetHmi3DBuf(MvHmi3dInfo *pBuf);

    int32_t SetHmi3DApaStateBuf(ApaStateToHmiTestInfo *pBuf);
    int32_t  SetHmi3DCanBuf(MvCanCarInfo *pBuf);
    int32_t  SetHmi3DSlotBuf(ApaAvapSlotOut *pBuf);
    int32_t  SetHmi3DOdBuf(MvApaAvapObjOut *pBuf);
    int32_t  SetHmi3DUssPdcBuf(ZU2UssSectorOutputData_t *pBuf);
    int32_t  SetHmi3DPlanTrackBuf(ApaPlanTrackInfo *pBuf);
    int32_t  SetHmi3DPlanFullPathBuf(PlanFullPath *pBuf);
    
    uint64_t GetCurrentTimestampMs(); 
private:
    int32_t IsEqualToZero(const float fData);
    void AddPbBaseHead(uint32_t uHead,uint32_t uDataType,PB_Hmi3dPackage* pb_hmi_3d);
    int32_t ConvertHmi3dPackagePB(const MvHmi3dInfo& tHmi3dInfo, PB_Hmi3dPackage* pb_hmi_3d);
    int32_t ConvertUssPdcToPB(const ZU2UssSectorOutputData_t& uss_pdc, PB_UssSectorOutputData* pb_uss_pdc);
    int32_t ConvertCanToPB(const MvCanCarInfo& src_can, PB_CanData* pb_can);
    int32_t ConvertApaStateToPB(const ApaStateToHmiTestInfo& src_apastate, PB_ApaStateInfo* pb_apastate);
    int32_t ConvertSlotToPB(const ApaAvapSlotOut& src_slot, PB_SlotInfo* pb_slot);
    int32_t ConvertOdToPB(const MvApaAvapObjOut& src_od, PB_OdInfo* pb_od);
    int32_t ConvertPlanTrackToPB(const ApaPlanTrackInfo& src_plantrack, PB_PlanTrackInfo* pb_plantrack);
    int32_t ConvertFullPathToPB(const PlanFullPath& src_fullpath, PB_PlanFullPath* pb_fullpath);

    
private:
    /*protobuf data*/
    //uss
    PB_UssSector_FrameHeader* p_frame_header;
    PB_UssSectorInfo* p_UssSectorInfo;
    PB_UssSectorOutputData pb_uss_pdc;
    //slot
    PB_PrkgSlot m_pb_prkslot;
    //od
    PB_AVAP_StcBarrier m_pb_stcbarrier;
    PB_AVAP_DynBarrier m_pb_dynbarrier;
    PB_AVAP_Speedbump m_pb_speedbump;
    //plantrack
    PB_PlanToHdmi m_pb_plan_to_hdmi;
    PB_Vector2D m_pb_vector2d;
    PB_TargetTrack m_pb_track;
    PB_Location m_pb_location;
    //fullpath
    PB_PathPointsInfo m_pb_pathpoints;
    PB_ApaPathPoint m_pb_point;

    //用于数据整包
    uint32_t m_sendPackageId;
    PB_Hmi3dPackage m_PB_Hmi3dPackage;
    PB_BaseHead* p_base_head;
    PB_UssSectorOutputData* p_uss_sector_data;
    PB_ApaStateInfo* p_apa_state_info;
    PB_CanData* p_can_car_info;
    PB_SlotInfo* p_apa_slot_out;
    PB_OdInfo* p_apa_obj_out;
    PB_PlanTrackInfo* p_plan_track_info;
    PB_PlanFullPath* p_plan_full_path;
    
    //Data cache
    Hmi3dBufQueue m_Hmi3dBufQueue;
    MvHmi3dInfo m_MvHmi3dInfo;
};














#endif