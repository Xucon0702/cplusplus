#include "hmi_3d_send.h"
#include <sys/types.h>
#include <sys/socket.h>

#include <chrono>

CHmi3DSendInf::CHmi3DSendInf()
{
    p_frame_header == NULL;
    p_UssSectorInfo == NULL;
    // p_frame_header == NULL;
    m_sendPackageId = 0;

    memset(&m_Hmi3dBufQueue,0,sizeof(Hmi3dBufQueue));
    memset(&m_MvHmi3dInfo,0,sizeof(MvHmi3dInfo));
}

CHmi3DSendInf::~CHmi3DSendInf()
{
    
}

int32_t CHmi3DSendInf::SendData(int sock,void *data,uint32_t size)
{
    if (sock < 0 || (data == NULL || size == 0))
    {
        return -1;
    }

//   if (send(sock, serialized_data.data(), serialized_data.size(), 0) == -1) {
//     std::cerr << "Failed to send data" << std::endl;
//     return -1;
//   }
  std::cout << "Sent data successfully" << std::endl;

    return 0;
}

int32_t CHmi3DSendInf::SendUssPdc(int sock, const ZU2UssSectorOutputData_t& uss_pdc)
{
    if (sock < 0)
    {
        return -1;
    }

    //转换数据
    // PB_UssSectorOutputData pb_uss_pdc;
    if(ConvertUssPdcToPB(uss_pdc,&pb_uss_pdc))
    {
        printf("fail to ConvertUssPdcToPB\n");
        return -1;
    }

    // 序列化protobuf消息
    std::string serialized_data;
    if (!pb_uss_pdc.SerializeToString(&serialized_data)) {
        std::cerr << "Failed to serialize PB_UssSectorOutputData." << std::endl;
        return -1;
    }
    
    // 发送序列化后的数据
    ssize_t bytes_sent = send(sock, serialized_data.data(), serialized_data.size(), 0);
    if (bytes_sent == -1) {
        std::cerr << "Failed to send data." << std::endl;
        return -1;
    }
    

    return 0;
}

// int32_t CHmi3DSendInf::SendHmi3dPackage(int sock, const ZU2UssSectorOutputData_t& uss_pdc)
int32_t CHmi3DSendInf::SendHmi3dPackage(int sock)
{
    if (sock < 0)
    {
        return -1;
    }

    //获取最新数据
    memset(&m_MvHmi3dInfo,0,sizeof(MvHmi3dInfo));
    GetHmi3DBuf(&m_MvHmi3dInfo);
    
    m_PB_Hmi3dPackage.Clear();    

    //转换数据
    if(ConvertHmi3dPackagePB(m_MvHmi3dInfo,&m_PB_Hmi3dPackage))
    {
        printf("fail to ConvertHmi3dPackagePB\n");
        return -1;
    }

    // 序列化protobuf消息
    std::string serialized_data;
    // serialized_data.clear();
    // if (!pb_uss_pdc.SerializeToString(&serialized_data)) {
    //     std::cerr << "Failed to serialize PB_UssSectorOutputData." << std::endl;
    //     return -1;
    // }
    
    if (!m_PB_Hmi3dPackage.SerializeToString(&serialized_data)) {
        std::cerr << "Failed to serialize PB_UssSectorOutputData." << std::endl;
        return -1;
    }

    // 发送序列化后的数据
    ssize_t bytes_sent = send(sock, serialized_data.data(), serialized_data.size(), 0);
    if (bytes_sent == -1) {
        printf("Failed to send data:%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }
    
    printf("sock = %d;serialized_data.size() = %ld;bytes_sent %ld\n",sock,serialized_data.size(),bytes_sent);

    m_sendPackageId++;

    return 0;
}

/*
return: 1: equal to zero; 0: not equal to zero
*/
int32_t CHmi3DSendInf::IsEqualToZero(const float fData)
{
    int32_t ret = 0;

    if(fData >= -0.000001 && fData <= 0.000001)
    {
        ret = 1;
    }

    return ret;
}

uint64_t CHmi3DSendInf::GetCurrentTimestampMs() 
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return static_cast<uint64_t>(milliseconds);
}

void CHmi3DSendInf::AddPbBaseHead(uint32_t uHead,uint32_t uDataType,PB_Hmi3dPackage* pb_hmi_3d)
{
    if(pb_hmi_3d == NULL)
    {
        printf("pb_hmi_3d is null\n");
        return ;
    }

    p_base_head = pb_hmi_3d->mutable_basehead();
    if(p_base_head != NULL)
    {
         p_base_head->Clear();
         p_base_head->set_crc_head(uHead);   
         p_base_head->set_data_type(uDataType);
    }
    else
    {
        printf("p_base_head is null\n");
    }

}

/*************************Convert data to protobuf************************************/
int32_t CHmi3DSendInf::ConvertHmi3dPackagePB(const MvHmi3dInfo& tHmi3dInfo, PB_Hmi3dPackage* pb_hmi_3d) {
        
    if(pb_hmi_3d == NULL)
    {
        printf("pb_hmi_3d is null\n");
        return -1;
    }

    pb_hmi_3d->set_lsendtimsms(GetCurrentTimestampMs());
    pb_hmi_3d->set_usendnum(m_sendPackageId);

    //add base head
    AddPbBaseHead(CRC_HEAD,TRANSPORT_3D_PACKAGE_PROTO,pb_hmi_3d);

    //ApaStateToHmiTestInfo
    p_apa_state_info = pb_hmi_3d->mutable_apastatedata();

    //MvCanCarInfo
    p_can_car_info = pb_hmi_3d->mutable_candata();

    //ApaAvapSlotOut
    p_apa_slot_out = pb_hmi_3d->mutable_slotdata();
    
    //MvApaAvapObjOut
    p_apa_obj_out = pb_hmi_3d->mutable_oddata();

    //ApaPlanTrackInfo
    p_plan_track_info = pb_hmi_3d->mutable_plantrackdata();

    //PlanFullPath
    p_plan_full_path = pb_hmi_3d->mutable_planfullpathdata();

    //ZU2UssSectorOutputData_t
    p_uss_sector_data = pb_hmi_3d->mutable_usssectordata();

    if(p_apa_state_info != NULL)
    {
        ConvertApaStateToPB(tHmi3dInfo.tApaStateInfo,p_apa_state_info);        
    }
    else
    {
        printf("p_apa_state_info is null\n");
    }

    if(p_can_car_info != NULL)
    {
        ConvertCanToPB(tHmi3dInfo.tMvCanCarInfo,p_can_car_info);        
    }
    else
    {
        printf("p_can_car_info is null\n");
    }
    
    if(p_apa_slot_out != NULL)
    {
        ConvertSlotToPB(tHmi3dInfo.tApaAvapSlotOut,p_apa_slot_out);        
    }
    else
    {
        printf("p_apa_slot_out is null\n");
    }
    
    if(p_apa_obj_out != NULL)
    {
        ConvertOdToPB(tHmi3dInfo.tMvApaAvapObjOut,p_apa_obj_out);        
    }
    else
    {
        printf("p_apa_obj_out is null\n");
    }
    
    if(p_plan_track_info != NULL)
    {
        ConvertPlanTrackToPB(tHmi3dInfo.tPlanTrackInfo,p_plan_track_info);        
    }
    else
    {
        printf("p_plan_track_info is null\n");
    }
    
    if(p_plan_full_path != NULL)
    {
        ConvertFullPathToPB(tHmi3dInfo.tPlanFullPath,p_plan_full_path);        
    }
    else
    {
        printf("p_pb_fullpath is null\n");
    }
    
    if(p_uss_sector_data != NULL)
    {
        ConvertUssPdcToPB(tHmi3dInfo.tUssPdc,p_uss_sector_data);        
    }
    else
    {
        printf("p_uss_sector_data is null\n");
    }

    return 0;
}

int32_t CHmi3DSendInf::ConvertUssPdcToPB(const ZU2UssSectorOutputData_t& uss_pdc, PB_UssSectorOutputData* pb_uss_pdc) {
        
    if(pb_uss_pdc == NULL)
    {
        printf("pb_uss_pdc is null\n");
        return -1;
    }

    pb_uss_pdc->Clear();

    // 转换FrameHead
    p_frame_header = pb_uss_pdc->mutable_framehead();
    if(p_frame_header == NULL)
    {
        printf("p_frame_header is null\n");
        return -1;
    }
    p_frame_header->set_nframeid(uss_pdc.FrameHead.nFrameId);
    // p_frame_header->set_nrervered(uss_pdc.FrameHead.nRervered);
    p_frame_header->set_itimemsec(uss_pdc.FrameHead.ITimeMsec);

    // 转换SectorData
    p_UssSectorInfo = pb_uss_pdc->mutable_sectordata();
    if(p_UssSectorInfo == NULL)
    {
        printf("p_UssSectorInfo is null\n");
        return -1;
    }

    p_UssSectorInfo->Clear();

    // for (int i = 0; i < 32; ++i) 
    for (int i = 0; i < 12; i++) 
    {
        // p_UssSectorInfo->add_pdc_distance((uss_pdc.SectorData.PDC_Distance[i])*F_CONVERT_RATE);
        p_UssSectorInfo->add_pdc_distance(uss_pdc.SectorData.LAEB_Distance[i]);
    }

    for (int i = 0; i < 12; i++) {
        p_UssSectorInfo->add_sensorstatus(uss_pdc.SectorData.SensorStatus[i]);
    }

    p_UssSectorInfo->set_systemstatus(uss_pdc.SectorData.SystemStatus);
    // p_UssSectorInfo->set_nrervered(uss_pdc.SectorData.nRervered);

    return 0;
}

int32_t CHmi3DSendInf::ConvertCanToPB(const MvCanCarInfo& src_can, PB_CanData* pb_can) {
        
    if(pb_can == NULL)
    {
        printf("pb_can is null\n");
        return -1;
    }
    pb_can->Clear();

    pb_can->set_nstructlen(src_can.nStructLen);
    pb_can->set_nframeid(src_can.nFrameId);
    pb_can->set_ltimemsec(src_can.lTimeMsec);
    pb_can->set_nbrake(src_can.nBrake);
    pb_can->set_nllight(src_can.nLLight);
    pb_can->set_nrlight(src_can.nRLight);
    pb_can->set_fvelocity(src_can.fVelocity);
    pb_can->set_nbrakevalue(src_can.nBrakeValue);
    pb_can->set_faccelpedalposition(src_can.fAccelPedalPosition);
    pb_can->set_flongiaccel(src_can.fLongiAccel);
    pb_can->set_fyawrate(src_can.fYawRate);
    pb_can->set_ntransmissionstatus(src_can.nTransmissionStatus);
    pb_can->set_falpha(src_can.fAlpha);
    pb_can->set_fsteeingwheelangle(src_can.fSteeingWheelAngle);
    pb_can->set_nparkbrake(src_can.nParkBrake);
    pb_can->set_nwipewasherswitch(src_can.nWipeWasherSwitch);
    pb_can->set_nhazardlightswitch(src_can.nHazardLightSwitch);
    pb_can->set_nhighlowbeamswitch(src_can.nHighLowBeamSwitch);

    pb_can->set_nflwheelspeedrc(src_can.nFLWheelSpeedRC);
    pb_can->set_nfrwheelspeedrc(src_can.nFRWheelSpeedRC);
    pb_can->set_nrlwheelspeedrc(src_can.nRLWheelSpeedRC);
    pb_can->set_nrrwheelspeedrc(src_can.nRRWheelSpeedRC);

    for (int i = 0; i < 4; i++) 
    {
        pb_can->add_ndoorstat(src_can.nDoorStat[i]);
    }

    pb_can->set_npulsedirection(src_can.nPulseDirection);
    pb_can->set_fwheelspd(src_can.fWheelSpd);
    pb_can->set_x(src_can.x);
    pb_can->set_y(src_can.y);
    pb_can->set_yaw(src_can.yaw);
    pb_can->set_nbonnetsts(src_can.nBonnetSts);
    pb_can->set_ntrunksts(src_can.nTrunkSts);
    pb_can->set_nsrfsts(src_can.nSRFSts);

    pb_can->set_bcm_keyst(src_can.BCM_KeySt);
    pb_can->set_acu_svm_button(src_can.ACU_SVM_Button);
    pb_can->set_acu_vr_svm_switch(src_can.ACU_VR_SVM_Switch);
    pb_can->set_hazardlampst(src_can.hazardLampSt);
    pb_can->set_externaltemperature(src_can.ExternalTemperature);
    pb_can->set_externaltemperaturevalidity(src_can.ExternalTemperatureValidity);
    pb_can->set_ltimemsecwheelpulse(src_can.lTimeMsecWheelPulse);
    pb_can->set_ltimemsecwheelspeed(src_can.lTimeMsecWheelSpeed);
    pb_can->set_ltimemsecsteerangle(src_can.lTimeMsecSteerAngle);
    pb_can->set_nflwheelspeedrc_vld(src_can.nFLWheelSpeedRC_Vld);
    pb_can->set_nfrwheelspeedrc_vld(src_can.nFRWheelSpeedRC_Vld);
    pb_can->set_nrlwheelspeedrc_vld(src_can.nRLWheelSpeedRC_Vld);
    pb_can->set_nrrwheelspeedrc_vld(src_can.nRRWheelSpeedRC_Vld);
    pb_can->set_nsteeingwheelratevld(src_can.nSteeingWheelRateVld);
    
    pb_can->set_nbrakevld(src_can.nBrakeVld);
    pb_can->set_nwhlspdvld_lf(src_can.nWhlSpdVld_LF);
    pb_can->set_naccelpedalpositionvld(src_can.nAccelPedalPositionVld);
    pb_can->set_yrs_lgtsnsrsts(src_can.YRS_LgtSnsrSts);
    pb_can->set_yrs_latsnsrsts(src_can.YRS_LatSnsrSts);
    pb_can->set_yrs_yawratesnsrsts(src_can.YRS_YawRateSnsrSts);
    pb_can->set_nsteeingwheelanglevld(src_can.nSteeingWheelAngleVld);
    pb_can->set_nrvslampoutpcmd(src_can.nRvsLampOutpCmd);

    pb_can->set_ltimesec(src_can.lTimeSec);
    pb_can->set_trailhook(src_can.TrailHook);
    pb_can->set_externalhumidity(src_can.ExternalHumidity);
    pb_can->set_externalhumidityvalidity(src_can.ExternalHumidityValidity);

    return 0;
}

int32_t CHmi3DSendInf::ConvertApaStateToPB(const ApaStateToHmiTestInfo& src_apastate, PB_ApaStateInfo* pb_apastate) {
        
    if(pb_apastate == NULL)
    {
        printf("pb_apastate is null\n");
        return -1;
    }
    pb_apastate->Clear();

    pb_apastate->set_ltimestampms(src_apastate.lTimestampMs);  // 时间戳
    pb_apastate->set_avap_apafundispsts(src_apastate.AVAP_APAFunDispSts);     // AVAP_APAFunDispSts
    pb_apastate->set_avap_infomsgs(src_apastate.AVAP_InfoMsgs);         // AVAP_InfoMsgs
    pb_apastate->set_avap_prkgpausemsgs(src_apastate.AVAP_PrkgPauseMsgs);    // AVAP_PrkgPauseMsgs
    pb_apastate->set_apa_cnclmsgs(src_apastate.APA_CnclMsgs);          // APA_CnclMsgs
    pb_apastate->set_apa_fctunavlmsgs(src_apastate.APA_FctUnavlMsgs);     // APA_FctUnavlMsgs    

    return 0;
}

int32_t CHmi3DSendInf::ConvertSlotToPB(const ApaAvapSlotOut& src_slot, PB_SlotInfo* pb_slot) {
        
    if(pb_slot == NULL)
    {
        printf("pb_slot is null\n");
        return -1;
    }

    pb_slot->Clear();
    pb_slot->set_timemsec(src_slot.TimeMsec);

    for (int i = 0; i < 16; i++) 
    {
        //过滤无效数据
        if(IsEqualToZero(src_slot.PrkgSlot[i].APA_SlotACoorX) && IsEqualToZero(src_slot.PrkgSlot[i].APA_SlotBCoorX))
        {
            continue;
        }

        // 设置PB_PrkgSlot消息
        m_pb_prkslot.set_apa_prkgslot_sts(src_slot.PrkgSlot[i].APA_PrkgSlot_Sts);
        m_pb_prkslot.set_apa_slotacoorx(src_slot.PrkgSlot[i].APA_SlotACoorX);
        m_pb_prkslot.set_apa_slotacoory(src_slot.PrkgSlot[i].APA_SlotACoorY);
        m_pb_prkslot.set_apa_slotbcoorx(src_slot.PrkgSlot[i].APA_SlotBCoorX);
        m_pb_prkslot.set_apa_slotbcoory(src_slot.PrkgSlot[i].APA_SlotBCoorY);
        m_pb_prkslot.set_apa_slotccoorx(src_slot.PrkgSlot[i].APA_SlotCCoorX);
        m_pb_prkslot.set_apa_slotccoory(src_slot.PrkgSlot[i].APA_SlotCCoorY);
        m_pb_prkslot.set_apa_slotdcoorx(src_slot.PrkgSlot[i].APA_SlotDCoorX);
        m_pb_prkslot.set_apa_slotdcoory(src_slot.PrkgSlot[i].APA_SlotDCoorY);

        pb_slot->add_prkgslot()->CopyFrom(m_pb_prkslot);

    }

    return 0;
}

int32_t CHmi3DSendInf::ConvertOdToPB(const MvApaAvapObjOut& src_od, PB_OdInfo* pb_od) {
        
    if(pb_od == NULL)
    {
        printf("pb_od is null\n");
        return -1;
    }

    pb_od->Clear();
    pb_od->set_timemsec(src_od.TimeMsec);

    // 设置PB_AVAP_StcBarrier消息
    for(int i=0;i<16;i++)
    {   
        if(src_od.AVAP_StcBarrier[i].AVAP_StcBarrierType == 0)
        {
            continue;
        }

        m_pb_stcbarrier.set_avap_stcbarriertype(src_od.AVAP_StcBarrier[i].AVAP_StcBarrierType);
        m_pb_stcbarrier.set_avap_stcbarrier_x(src_od.AVAP_StcBarrier[i].AVAP_StcBarrier_X);
        m_pb_stcbarrier.set_avap_stcbarrier_y(src_od.AVAP_StcBarrier[i].AVAP_StcBarrier_Y);
        m_pb_stcbarrier.set_avap_stcbarrierdrct(src_od.AVAP_StcBarrier[i].AVAP_StcBarrierDrct);

        //添加PB_AVAP_StcBarrier到PB_OdInfo
        pb_od->add_astcbarrier()->CopyFrom(m_pb_stcbarrier);
    }
    
    //设置PB_AVAP_DynBarrier消息
    for(int i=0;i<16;i++)
    {   
        if(src_od.AVAP_DynBarrier[i].AVAP_DynBarrierType == 0)
        {
            continue;
        }

        m_pb_dynbarrier.set_avap_dynbarriertype(src_od.AVAP_DynBarrier[i].AVAP_DynBarrierType);
        m_pb_dynbarrier.set_avap_dynbarrier_x(src_od.AVAP_DynBarrier[i].AVAP_DynBarrier_X);
        m_pb_dynbarrier.set_avap_dynbarrier_y(src_od.AVAP_DynBarrier[i].AVAP_DynBarrier_Y);
        m_pb_dynbarrier.set_avap_dynbarrierdrct(src_od.AVAP_DynBarrier[i].AVAP_DynBarrierDrct);

        //添加PB_AVAP_StcBarrier到PB_OdInfo
        pb_od->add_adynbarrier()->CopyFrom(m_pb_dynbarrier);
    }

    // 设置PB_AVAP_Speedbump消息
    for(int i=0;i<3;i++)
    {   
        if(IsEqualToZero(src_od.AVAP_Speedbump[i].AVAP_Speedbump_A_X) && \
            IsEqualToZero(src_od.AVAP_Speedbump[i].AVAP_Speedbump_A_Y) && \
            IsEqualToZero(src_od.AVAP_Speedbump[i].AVAP_Speedbump_B_X) && \
            IsEqualToZero(src_od.AVAP_Speedbump[i].AVAP_Speedbump_B_Y))
        {
            continue;
        }

        m_pb_speedbump.set_avap_speedbump_a_x(src_od.AVAP_Speedbump[i].AVAP_Speedbump_A_X);
        m_pb_speedbump.set_avap_speedbump_a_y(src_od.AVAP_Speedbump[i].AVAP_Speedbump_A_Y);
        m_pb_speedbump.set_avap_speedbump_b_x(src_od.AVAP_Speedbump[i].AVAP_Speedbump_B_X);
        m_pb_speedbump.set_avap_speedbump_b_y(src_od.AVAP_Speedbump[i].AVAP_Speedbump_B_Y);

        //添加PB_AVAP_StcBarrier到PB_OdInfo
        pb_od->add_aspeedbump()->CopyFrom(m_pb_speedbump);
    }
    
    return 0;
}

int32_t CHmi3DSendInf::ConvertPlanTrackToPB(const ApaPlanTrackInfo& src_plantrack, PB_PlanTrackInfo* pb_plantrack) {
        
    if(pb_plantrack == NULL)
    {
        printf("pb_plantrack is null\n");
        return -1;
    }

    pb_plantrack->Clear();
    m_pb_plan_to_hdmi.Clear();

    pb_plantrack->set_ltimemsec(src_plantrack.lTimeMsec);

    uint32_t validnum = src_plantrack.validNum > 400?400:src_plantrack.validNum;

    pb_plantrack->set_validnum(validnum);

    m_pb_plan_to_hdmi.set_ucmd(src_plantrack.tPlanToHdmi.uCmd);
    m_pb_plan_to_hdmi.set_ustatue(src_plantrack.tPlanToHdmi.uStatue);
    m_pb_plan_to_hdmi.set_utotalstep(src_plantrack.tPlanToHdmi.uTotalStep);
    m_pb_plan_to_hdmi.set_ucurrentstep(src_plantrack.tPlanToHdmi.uCurrentStep);
    m_pb_plan_to_hdmi.set_udisstep(src_plantrack.tPlanToHdmi.uDisStep);
    m_pb_plan_to_hdmi.set_uplanoutdetectsts(src_plantrack.tPlanToHdmi.uPlanOutDetectSts);
    for(int i=0;i<4;i++)
    {
        m_pb_plan_to_hdmi.add_px(src_plantrack.tPlanToHdmi.px[i]);
        m_pb_plan_to_hdmi.add_py(src_plantrack.tPlanToHdmi.py[i]);
    }    
    pb_plantrack->mutable_tplantohdmi()->CopyFrom(m_pb_plan_to_hdmi);

    for(int i=0;i < validnum;i++)
    {
        m_pb_vector2d.set_x(src_plantrack.aTracksPlan[i].point.x);
        m_pb_vector2d.set_y(src_plantrack.aTracksPlan[i].point.y);
        m_pb_track.mutable_point()->CopyFrom(m_pb_vector2d);
        m_pb_track.set_yaw(src_plantrack.aTracksPlan[i].yaw);
        m_pb_track.set_curvature(src_plantrack.aTracksPlan[i].curvature);
        m_pb_track.set_speed(src_plantrack.aTracksPlan[i].speed);
        m_pb_track.set_ndetectstates(src_plantrack.aTracksPlan[i].nDetectStates);
        pb_plantrack->add_atracksplan()->CopyFrom(m_pb_track);
    }
    
    m_pb_location.set_x(src_plantrack.syncDr.x);
    m_pb_location.set_y(src_plantrack.syncDr.y);
    m_pb_location.set_yaw(src_plantrack.syncDr.yaw);
    pb_plantrack->mutable_syncdr()->CopyFrom(m_pb_location);

    return 0;
}

int32_t CHmi3DSendInf::ConvertFullPathToPB(const PlanFullPath& src_fullpath, PB_PlanFullPath* pb_fullpath) {
        
    if(pb_fullpath == NULL)
    {
        printf("pb_fullpath is null\n");
        return -1;
    }

    pb_fullpath->Clear();

    uint32_t uValidPathSize = 0;
    uint32_t uValidPointSize = 0;

    pb_fullpath->set_timemsec(src_fullpath.lTimeMsec);

    // 设置其他字段
    pb_fullpath->set_apatotalstep(src_fullpath.uint8_APATotalStep);
    pb_fullpath->set_apacurstep(src_fullpath.uint8_APACurStep);
    pb_fullpath->set_apapathgear(src_fullpath.uint8_APAPathGear);
    // pb_fullpath->set_reserved(src_fullpath.uint8_APATotalStep);
    pb_fullpath->set_apapathlen(src_fullpath.uint16_APAPathLen);
    pb_fullpath->set_aparmpathlen(src_fullpath.uint16_APARmPathLen);
    pb_fullpath->set_stopdistance(src_fullpath.uint16_StopDistance);

    uValidPathSize = src_fullpath.validNum;
    pb_fullpath->set_validnum(uValidPathSize);

    for(int i = 0; i < uValidPathSize;i++)
    {
        uValidPointSize = src_fullpath.Array_APAPathPoints[i].validNum;
        m_pb_pathpoints.Clear();
        for(int j=0;j<uValidPointSize;j++)
        {
            m_pb_point.set_fx(src_fullpath.Array_APAPathPoints[i].aPoints[j].fX);
            m_pb_point.set_fy(src_fullpath.Array_APAPathPoints[i].aPoints[j].fY);
            m_pb_point.set_fcurv(src_fullpath.Array_APAPathPoints[i].aPoints[j].fCurv);
            m_pb_pathpoints.add_apoints()->CopyFrom(m_pb_point);
        }
        pb_fullpath->add_array_apapathpoints()->CopyFrom(m_pb_pathpoints);
    }

    return 0;
}

/***************************DATA CACHE*****************************/
int32_t  CHmi3DSendInf::CleanHmi3DQueue()
{
    memset(&m_Hmi3dBufQueue,0,sizeof(Hmi3dBufQueue));
    memset(&m_MvHmi3dInfo,0,sizeof(MvHmi3dInfo));
    return 0;
}

/*
取缓存队列中最新数据,后期可添加时间戳匹配接口
*/
int32_t  CHmi3DSendInf::GetHmi3DBuf(MvHmi3dInfo *pBuf)
{
    if(pBuf == NULL)
    {
        printf("MvHmi3dInfo* is null\n");
        return -1;
    }

	int32_t nRet = 0;
	uint32_t uReadIndex = 0;

    memset(pBuf,0,sizeof(MvHmi3dInfo));

    //get ApaStateToHmiTestInfo
    uReadIndex = m_Hmi3dBufQueue.tApaStateInfoQueue.nReadId.load();
    memcpy(&(pBuf->tApaStateInfo), &m_Hmi3dBufQueue.tApaStateInfoQueue.aApaStateInfo[uReadIndex], sizeof(ApaStateToHmiTestInfo));

    //get MvCanCarInfo
    uReadIndex = m_Hmi3dBufQueue.tCanInfoQueue.nReadId.load();
    memcpy(&(pBuf->tMvCanCarInfo), &m_Hmi3dBufQueue.tCanInfoQueue.aCanInfo[uReadIndex], sizeof(MvCanCarInfo));

    //get ApaAvapSlotOut
    uReadIndex = m_Hmi3dBufQueue.tSlotInfoQueue.nReadId.load();
    memcpy(&(pBuf->tApaAvapSlotOut), &m_Hmi3dBufQueue.tSlotInfoQueue.aSlotInfo[uReadIndex], sizeof(ApaAvapSlotOut));

    //get MvApaAvapObjOut
    uReadIndex = m_Hmi3dBufQueue.tOdInfoQueue.nReadId.load();
    memcpy(&(pBuf->tMvApaAvapObjOut), &m_Hmi3dBufQueue.tOdInfoQueue.aOdInfo[uReadIndex], sizeof(MvApaAvapObjOut));

    //get ZU2UssSectorOutputData_t
    uReadIndex = m_Hmi3dBufQueue.tUssInfoQueue.nReadId.load();
    memcpy(&(pBuf->tUssPdc), &m_Hmi3dBufQueue.tUssInfoQueue.aUssPdcInfo[uReadIndex], sizeof(ZU2UssSectorOutputData_t));

    //get ApaPlanTrackInfo
    uReadIndex = m_Hmi3dBufQueue.tPlanTrackQueue.nReadId.load();
    memcpy(&(pBuf->tPlanTrackInfo), &m_Hmi3dBufQueue.tPlanTrackQueue.aPlanTrack[uReadIndex], sizeof(ApaPlanTrackInfo));

    //get PlanFullPath
    uReadIndex = m_Hmi3dBufQueue.tPlanFullPathQueue.nReadId.load();
    memcpy(&(pBuf->tPlanFullPath), &m_Hmi3dBufQueue.tPlanFullPathQueue.aPlanFullPath[uReadIndex], sizeof(PlanFullPath));
	
	return nRet;
}
int32_t  CHmi3DSendInf::SetHmi3DApaStateBuf(ApaStateToHmiTestInfo *pBuf)
{
	int32_t nRet = 0;
    uint32_t uNextWRId = 0;

    if(pBuf == NULL)
    {
        printf("pBuf is null:%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

    uNextWRId = (m_Hmi3dBufQueue.tApaStateInfoQueue.nWriteId.load() + 1)%MAX_QUEUE_SIZE;
    memcpy(&m_Hmi3dBufQueue.tApaStateInfoQueue.aApaStateInfo[m_Hmi3dBufQueue.tApaStateInfoQueue.nWriteId.load()],pBuf,sizeof(ApaStateToHmiTestInfo));
    if(m_Hmi3dBufQueue.tApaStateInfoQueue.nReadId.load() != m_Hmi3dBufQueue.tApaStateInfoQueue.nWriteId.load())
    {
        //更新最新数据的nReadId
        m_Hmi3dBufQueue.tApaStateInfoQueue.nReadId.store(m_Hmi3dBufQueue.tApaStateInfoQueue.nWriteId.load());
    }

    //更新下一个数据的nWriteId
    m_Hmi3dBufQueue.tApaStateInfoQueue.nWriteId.store(uNextWRId);

	return nRet;
}
int32_t  CHmi3DSendInf::SetHmi3DCanBuf(MvCanCarInfo *pBuf)
{
	int32_t nRet = 0;
    uint32_t uNextWRId = 0;

    if(pBuf == NULL)
    {
        printf("pBuf is null:%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

    uNextWRId = (m_Hmi3dBufQueue.tCanInfoQueue.nWriteId.load() + 1)%MAX_QUEUE_SIZE;
    memcpy(&m_Hmi3dBufQueue.tCanInfoQueue.aCanInfo[m_Hmi3dBufQueue.tCanInfoQueue.nWriteId.load()],pBuf,sizeof(MvCanCarInfo));
    if(m_Hmi3dBufQueue.tCanInfoQueue.nReadId.load() != m_Hmi3dBufQueue.tCanInfoQueue.nWriteId.load())
    {
        //更新最新数据的nReadId
        m_Hmi3dBufQueue.tCanInfoQueue.nReadId.store(m_Hmi3dBufQueue.tCanInfoQueue.nWriteId.load());
    }

    //更新下一个数据的nWriteId
    m_Hmi3dBufQueue.tCanInfoQueue.nWriteId.store(uNextWRId);

	return nRet;
}
int32_t  CHmi3DSendInf::SetHmi3DSlotBuf(ApaAvapSlotOut *pBuf)
{
	int32_t nRet = 0;
    uint32_t uNextWRId = 0;

    if(pBuf == NULL)
    {
        printf("pBuf is null:%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

    uNextWRId = (m_Hmi3dBufQueue.tSlotInfoQueue.nWriteId.load() + 1)%MAX_QUEUE_SIZE;
    memcpy(&m_Hmi3dBufQueue.tSlotInfoQueue.aSlotInfo[m_Hmi3dBufQueue.tSlotInfoQueue.nWriteId.load()],pBuf,sizeof(ApaAvapSlotOut));
    if(m_Hmi3dBufQueue.tSlotInfoQueue.nReadId.load() != m_Hmi3dBufQueue.tSlotInfoQueue.nWriteId.load())
    {
        //更新最新数据的nReadId
        m_Hmi3dBufQueue.tSlotInfoQueue.nReadId.store(m_Hmi3dBufQueue.tSlotInfoQueue.nWriteId.load());
    }

    //更新下一个数据的nWriteId
    m_Hmi3dBufQueue.tSlotInfoQueue.nWriteId.store(uNextWRId);

	return nRet;
}

int32_t  CHmi3DSendInf::SetHmi3DOdBuf(MvApaAvapObjOut *pBuf)
{
	int32_t nRet = 0;
    uint32_t uNextWRId = 0;

    if(pBuf == NULL)
    {
        printf("pBuf is null:%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

    uNextWRId = (m_Hmi3dBufQueue.tOdInfoQueue.nWriteId.load() + 1)%MAX_QUEUE_SIZE;
    memcpy(&m_Hmi3dBufQueue.tOdInfoQueue.aOdInfo[m_Hmi3dBufQueue.tOdInfoQueue.nWriteId.load()],pBuf,sizeof(MvApaAvapObjOut));
    if(m_Hmi3dBufQueue.tOdInfoQueue.nReadId.load() != m_Hmi3dBufQueue.tOdInfoQueue.nWriteId.load())
    {
        //更新最新数据的nReadId
        m_Hmi3dBufQueue.tOdInfoQueue.nReadId.store(m_Hmi3dBufQueue.tOdInfoQueue.nWriteId.load());
    }

    //更新下一个数据的nWriteId
    m_Hmi3dBufQueue.tOdInfoQueue.nWriteId.store(uNextWRId);

	return nRet;
}

int32_t  CHmi3DSendInf::SetHmi3DUssPdcBuf(ZU2UssSectorOutputData_t *pBuf)
{
	int32_t nRet = 0;
    uint32_t uNextWRId = 0;

    if(pBuf == NULL)
    {
        printf("pBuf is null:%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

    uNextWRId = (m_Hmi3dBufQueue.tUssInfoQueue.nWriteId.load() + 1)%MAX_QUEUE_SIZE;
    memcpy(&m_Hmi3dBufQueue.tUssInfoQueue.aUssPdcInfo[m_Hmi3dBufQueue.tUssInfoQueue.nWriteId.load()],pBuf,sizeof(ZU2UssSectorOutputData_t));
    if(m_Hmi3dBufQueue.tUssInfoQueue.nReadId.load() != m_Hmi3dBufQueue.tUssInfoQueue.nWriteId.load())
    {
        //更新最新数据的nReadId
        m_Hmi3dBufQueue.tUssInfoQueue.nReadId.store(m_Hmi3dBufQueue.tUssInfoQueue.nWriteId.load());
    }

    //更新下一个数据的nWriteId
    m_Hmi3dBufQueue.tUssInfoQueue.nWriteId.store(uNextWRId);

	return nRet;
}

int32_t  CHmi3DSendInf::SetHmi3DPlanTrackBuf(ApaPlanTrackInfo *pBuf)
{
	int32_t nRet = 0;
    uint32_t uNextWRId = 0;

    if(pBuf == NULL)
    {
        printf("pBuf is null:%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

    uNextWRId = (m_Hmi3dBufQueue.tPlanTrackQueue.nWriteId.load() + 1)%MAX_QUEUE_SIZE;
    memcpy(&m_Hmi3dBufQueue.tPlanTrackQueue.aPlanTrack[m_Hmi3dBufQueue.tPlanTrackQueue.nWriteId.load()],pBuf,sizeof(ApaPlanTrackInfo));
    if(m_Hmi3dBufQueue.tPlanTrackQueue.nReadId.load() != m_Hmi3dBufQueue.tPlanTrackQueue.nWriteId.load())
    {
        //更新最新数据的nReadId
        m_Hmi3dBufQueue.tPlanTrackQueue.nReadId.store(m_Hmi3dBufQueue.tPlanTrackQueue.nWriteId.load());
    }

    //更新下一个数据的nWriteId
    m_Hmi3dBufQueue.tPlanTrackQueue.nWriteId.store(uNextWRId);

	return nRet;
}

int32_t  CHmi3DSendInf::SetHmi3DPlanFullPathBuf(PlanFullPath *pBuf)
{
	int32_t nRet = 0;
    uint32_t uNextWRId = 0;

    if(pBuf == NULL)
    {
        printf("pBuf is null:%s,%d\n",__FUNCTION__,__LINE__);
        return -1;
    }

    uNextWRId = (m_Hmi3dBufQueue.tPlanFullPathQueue.nWriteId.load() + 1)%MAX_QUEUE_SIZE;
    memcpy(&m_Hmi3dBufQueue.tPlanFullPathQueue.aPlanFullPath[m_Hmi3dBufQueue.tPlanFullPathQueue.nWriteId.load()],pBuf,sizeof(PlanFullPath));
    if(m_Hmi3dBufQueue.tPlanFullPathQueue.nReadId.load() != m_Hmi3dBufQueue.tPlanFullPathQueue.nWriteId.load())
    {
        //更新最新数据的nReadId
        m_Hmi3dBufQueue.tPlanFullPathQueue.nReadId.store(m_Hmi3dBufQueue.tPlanFullPathQueue.nWriteId.load());
    }

    //更新下一个数据的nWriteId
    m_Hmi3dBufQueue.tPlanFullPathQueue.nWriteId.store(uNextWRId);

	return nRet;
}