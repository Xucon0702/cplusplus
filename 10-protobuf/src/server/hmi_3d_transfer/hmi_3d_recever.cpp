#include "hmi_3d_recever.h"

#include <iostream>
#include <string>
#include <vector>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

CHmi3DReceiver::CHmi3DReceiver()
{
    memset(&m_hmi_3d_info,0,sizeof(MvHmi3dInfo));
}

CHmi3DReceiver::~CHmi3DReceiver()
{
    
}

int32_t CHmi3DReceiver::CheckData(PB_Hmi3dPackage* pb_hmi_3d_package,uint32_t dataType)
{
     if(pb_hmi_3d_package == NULL)
    {
        printf("%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

    p_recved_head = pb_hmi_3d_package->mutable_basehead();

    if(p_recved_head == NULL)
    {
        printf("%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

    if((p_recved_head->crc_head() != CRC_HEAD)||(p_recved_head->data_type() != dataType))
    {
        printf("CheckData wrong;%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

    return 0;
}

int32_t CHmi3DReceiver::ReceiveUssPdc(int sock, PB_UssSectorOutputData* pb_uss_pdc) {
    if(pb_uss_pdc == NULL)
    {
        std::cerr << "pb_uss_pdc is null" << std::endl;
        return -1;
    }
    
    // 接收数据
    // char buffer[1024];
    ssize_t bytes_received = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        std::cerr << "Failed to receive data." << std::endl;
        return -1;
    }

    // 解析protobuf消息
    if (!pb_uss_pdc->ParseFromArray(buffer, bytes_received)) {
        std::cerr << "Failed to parse PB_UssSectorOutputData." << std::endl;
        return -1;
    }

    // 打印解析结果
    PrintPB(pb_uss_pdc);

    return bytes_received;
}

int32_t CHmi3DReceiver::RecvHmi3dPackage(int sock, PB_Hmi3dPackage* pb_hmi_3d_package) {

    printf("RecvHmi3dPackage\n");

    if(pb_hmi_3d_package == NULL)
    {
        std::cerr << "pb_hmi_3d_package is null" << std::endl;
        return -1;
    }
    
    // 接收数据
    // char buffer[1024];
    ssize_t bytes_received = recv(sock, buffer, MAX_HMI_3D_INFO_LEN, 0);
    if (bytes_received == -1) {
        std::cerr << "Failed to receive data." << std::endl;
        return -1;
    }

    // 解析protobuf消息
    if (!pb_hmi_3d_package->ParseFromArray(buffer, bytes_received)) {
        std::cerr << "Failed to parse pb_hmi_3d_package." << std::endl;
        return -1;
    }

    if(CheckData(pb_hmi_3d_package,TRANSPORT_3D_PACKAGE_PROTO) != 0)
    {
        printf("wrong PB_Hmi3dPackage data\n");
        return 0;
    }

    //转为内部结构体数据
    ConvertHmi3dPackage(pb_hmi_3d_package, &m_hmi_3d_info);
    // 打印解析结果
    // PrintPbHmi3dPackage(*pb_hmi_3d_package);

    return bytes_received;
}

void CHmi3DReceiver::PrintPB(const PB_UssSectorOutputData* pb_uss_pdc) {
    const auto& frame_header = pb_uss_pdc->framehead();
    const auto& sector_info = pb_uss_pdc->sectordata();

    std::cout << "Frame Head:" << std::endl;
    std::cout << "  nFrameId: " << frame_header.nframeid() << std::endl;
    // std::cout << "  nRervered: " << frame_header.nrervered() << std::endl;
    std::cout << "  ITimeMsec: " << frame_header.itimemsec() << std::endl;

    std::cout << "Sector Data:" << std::endl;
    std::cout << "  PDC Distance: ";
    for (int i = 0; i < sector_info.pdc_distance_size(); i++) {
        std::cout << sector_info.pdc_distance(i) << " ";
    }
    std::cout << std::endl;

    // std::cout << "  LAEB Distance: ";
    // for (int i = 0; i < sector_info.laeb_distance_size(); ++i) {
    //     std::cout << sector_info.laeb_distance(i) << " ";
    // }
    // std::cout << std::endl;

    std::cout << "  Sensor Status: ";
    for (int i = 0; i < sector_info.sensorstatus_size(); i++) {
        std::cout << static_cast<int>(sector_info.sensorstatus(i)) << " ";
    }
    std::cout << std::endl;

    std::cout << "  System Status: " << static_cast<int>(sector_info.systemstatus()) << std::endl;
    // std::cout << "  nRervered: " << sector_info.nrervered() << std::endl;
}

void CHmi3DReceiver::PrintPbHmi3dPackage(const PB_Hmi3dPackage& package) {
    std::cout << "BaseHead: " << package.basehead().DebugString() << std::endl;
    std::cout << "LSendTimsMs: " << package.lsendtimsms() << std::endl;
    std::cout << "uSendNum: " << package.usendnum() << std::endl;

    if (package.has_candata()) {
        std::cout << "CanData: " << package.candata().DebugString() << std::endl;
    }

    if (package.has_usssectordata()) {
        std::cout << "UssSectorData: " << package.usssectordata().DebugString() << std::endl;
    }

    if (package.has_apastatedata()) {
        std::cout << "ApaStateData: " << package.apastatedata().DebugString() << std::endl;
    }

    if (package.has_slotdata()) {
        std::cout << "SlotData: " << package.slotdata().DebugString() << std::endl;
    }

    if (package.has_oddata()) {
        std::cout << "OdData: " << package.oddata().DebugString() << std::endl;
    }

    if (package.has_plantrackdata()) {
        // std::cout << "PlanTrackData: " << package.plantrackdata().DebugString() << std::endl;
    }

    if (package.has_planfullpathdata()) {
        // std::cout << "PlanFullPathData: " << package.planfullpathdata().DebugString() << std::endl;
    }
}

void CHmi3DReceiver::ConvertHmi3dPackage(PB_Hmi3dPackage* pb_hmi_3d_package, MvHmi3dInfo* p_mv_3d_info)
{
    if(pb_hmi_3d_package == NULL || p_mv_3d_info == NULL)
    {
        printf("%s,%d\n",__FUNCTION__,__LINE__);
        return;
    }

    uint64_t LSendTimsMs = pb_hmi_3d_package->lsendtimsms();
    uint32_t uSendNum = pb_hmi_3d_package->usendnum();

    printf("\n LSendTimsMs %ld;uSendNum %u\n",LSendTimsMs,uSendNum);

    if (pb_hmi_3d_package->has_candata()) {
        //转换为MvCanCarInfo
        ConvertToCanData(pb_hmi_3d_package->candata(), &p_mv_3d_info->tMvCanCarInfo);
    }

    if (pb_hmi_3d_package->has_usssectordata()) {
        ConvertToUssPdc(pb_hmi_3d_package->usssectordata(), &p_mv_3d_info->tUssPdc);
    }

    if (pb_hmi_3d_package->has_apastatedata()) {
        ConvertToApaStateInfo(pb_hmi_3d_package->apastatedata(), &p_mv_3d_info->tApaStateInfo);
    }

    if (pb_hmi_3d_package->has_slotdata()) {
        ConvertToSlotData(pb_hmi_3d_package->slotdata(), &p_mv_3d_info->tApaAvapSlotOut);
    }

    if (pb_hmi_3d_package->has_oddata()) {
        ConvertToOdData(pb_hmi_3d_package->oddata(), &p_mv_3d_info->tMvApaAvapObjOut);
    }

    if (pb_hmi_3d_package->has_plantrackdata()) {
        ConvertToPlanTrackData(pb_hmi_3d_package->plantrackdata(), &p_mv_3d_info->tPlanTrackInfo);
    }

    if (pb_hmi_3d_package->has_planfullpathdata()) {
        ConvertToPlanFullPathData(pb_hmi_3d_package->planfullpathdata(), &p_mv_3d_info->tPlanFullPath);
    }
}   

void CHmi3DReceiver::ConvertToCanData(const PB_CanData& pb_candata,MvCanCarInfo* p_can)
{
    if(p_can == NULL)
    {
        printf("%s,%d\n",__FUNCTION__,__LINE__);
        return;
    }

    p_can->lTimeMsec = pb_candata.ltimemsec();
    p_can->nBrake = pb_candata.nbrake();

    printf("p_can:lTimeMsec %ld,nBrake %d\n",p_can->lTimeMsec,p_can->nBrake);

    for(int i = 0; i < pb_candata.ndoorstat_size(); i++)
    {
        p_can->nDoorStat[i] = pb_candata.ndoorstat(i);
        printf("nDoorStat[%d]:%d\n",i,p_can->nDoorStat[i]);
    }
}
void CHmi3DReceiver::ConvertToUssPdc(const PB_UssSectorOutputData& pb_ussPdcData,ZU2UssSectorOutputData_t* p_ussPdc)
{
    if(p_ussPdc == NULL)
    {
        printf("%s,%d\n",__FUNCTION__,__LINE__);
        return;
    }
    p_ussPdc->FrameHead.ITimeMsec = pb_ussPdcData.framehead().itimemsec();
    p_ussPdc->FrameHead.nFrameId = pb_ussPdcData.framehead().nframeid();
    p_ussPdc->SectorData.SystemStatus = pb_ussPdcData.sectordata().systemstatus();

    printf("p_ussPdc:ITimeMsec %ld,nFrameId %u,SystemStatus %d\n",p_ussPdc->FrameHead.ITimeMsec,p_ussPdc->FrameHead.nFrameId,p_ussPdc->SectorData.SystemStatus);

    for(int i = 0; i < pb_ussPdcData.sectordata().pdc_distance_size(); i++)
    {
        p_ussPdc->SectorData.PDC_Distance[i] = pb_ussPdcData.sectordata().pdc_distance(i);
        p_ussPdc->SectorData.SensorStatus[i] = pb_ussPdcData.sectordata().sensorstatus(i);

        printf("PDC_Distance[%d]:%d,SensorStatus[%d]:%d\n",i,p_ussPdc->SectorData.PDC_Distance[i],i,p_ussPdc->SectorData.SensorStatus[i]);
    }
    
}
void CHmi3DReceiver::ConvertToApaStateInfo(const PB_ApaStateInfo& pb_apaStateInfo,ApaStateToHmiTestInfo* p_apaStateInfo)
{
    if(p_apaStateInfo == NULL)
    {
        printf("%s,%d\n",__FUNCTION__,__LINE__);
        return;
    }

    p_apaStateInfo->lTimestampMs = pb_apaStateInfo.ltimestampms();
    p_apaStateInfo->AVAP_APAFunDispSts = pb_apaStateInfo.avap_apafundispsts();
    p_apaStateInfo->AVAP_InfoMsgs = pb_apaStateInfo.avap_infomsgs();
    p_apaStateInfo->AVAP_PrkgPauseMsgs = pb_apaStateInfo.avap_prkgpausemsgs();
    p_apaStateInfo->APA_CnclMsgs = pb_apaStateInfo.apa_cnclmsgs();
    p_apaStateInfo->APA_FctUnavlMsgs = pb_apaStateInfo.apa_fctunavlmsgs();

    printf("p_apaStateInfo:lTimestampMs %ld,AVAP_APAFunDispSts %d,AVAP_InfoMsgs %d,AVAP_PrkgPauseMsgs %d,APA_CnclMsgs %d,APA_FctUnavlMsgs %d\n",\
    p_apaStateInfo->lTimestampMs,p_apaStateInfo->AVAP_APAFunDispSts,p_apaStateInfo->AVAP_InfoMsgs,\
    p_apaStateInfo->AVAP_PrkgPauseMsgs,p_apaStateInfo->APA_CnclMsgs,p_apaStateInfo->APA_FctUnavlMsgs);

}

void CHmi3DReceiver::ConvertToSlotData(const PB_SlotInfo& pb_slot,ApaAvapSlotOut* p_slot)
{
    if(p_slot == NULL)
    {
        printf("%s,%d\n",__FUNCTION__,__LINE__);
        return;
    }

    p_slot->TimeMsec = pb_slot.timemsec();
    for(int i = 0;i < pb_slot.prkgslot_size();i++)
    {
        p_slot->PrkgSlot[i].APA_PrkgSlot_Sts = pb_slot.prkgslot(i).apa_prkgslot_sts();
        p_slot->PrkgSlot[i].APA_SlotACoorX = pb_slot.prkgslot(i).apa_slotacoorx(); 
        p_slot->PrkgSlot[i].APA_SlotACoorY = pb_slot.prkgslot(i).apa_slotacoory(); 
        p_slot->PrkgSlot[i].APA_SlotBCoorX = pb_slot.prkgslot(i).apa_slotbcoorx(); 
        p_slot->PrkgSlot[i].APA_SlotBCoorY = pb_slot.prkgslot(i).apa_slotbcoory(); 
        p_slot->PrkgSlot[i].APA_SlotCCoorX = pb_slot.prkgslot(i).apa_slotccoorx(); 
        p_slot->PrkgSlot[i].APA_SlotCCoorY = pb_slot.prkgslot(i).apa_slotccoory(); 
        p_slot->PrkgSlot[i].APA_SlotDCoorX = pb_slot.prkgslot(i).apa_slotdcoorx(); 
        p_slot->PrkgSlot[i].APA_SlotDCoorY = pb_slot.prkgslot(i).apa_slotdcoory();  

        printf("p_slot:TimeMsec %ld,APA_PrkgSlot_Sts %d,(%d,%d)(%d,%d)(%d,%d)(%d,%d)\n",\
        p_slot->TimeMsec,p_slot->PrkgSlot[i].APA_PrkgSlot_Sts,p_slot->PrkgSlot[i].APA_SlotACoorX,p_slot->PrkgSlot[i].APA_SlotACoorY,\
        p_slot->PrkgSlot[i].APA_SlotBCoorX,p_slot->PrkgSlot[i].APA_SlotBCoorY,p_slot->PrkgSlot[i].APA_SlotCCoorX,p_slot->PrkgSlot[i].APA_SlotCCoorY,\
        p_slot->PrkgSlot[i].APA_SlotDCoorX,p_slot->PrkgSlot[i].APA_SlotDCoorY);
    }
}

void CHmi3DReceiver::ConvertToOdData(const PB_OdInfo& pb_odInfo,MvApaAvapObjOut* p_od)
{
    if(p_od == NULL)
    {
        printf("%s,%d\n",__FUNCTION__,__LINE__);
        return;
    }
}

void CHmi3DReceiver::ConvertToPlanTrackData(const PB_PlanTrackInfo& pb_planTrackInfo,ApaPlanTrackInfo* p_planTrackInfo)
{
    if(p_planTrackInfo == NULL)
    {
        printf("%s,%d\n",__FUNCTION__,__LINE__);
        return;
    }
}

void CHmi3DReceiver::ConvertToPlanFullPathData(const PB_PlanFullPath& pb_planFullPath,PlanFullPath* p_planFullPath)
{
    if(p_planFullPath == NULL)
    {
        printf("%s,%d\n",__FUNCTION__,__LINE__);
        return;
    }
}