#include "hmi_3d_transfer/hmi_send_3d.h"
#include <sys/types.h>
#include <sys/socket.h>

#include <chrono>


CHmiSend3DInf::CHmiSend3DInf()
{

}

CHmiSend3DInf::~CHmiSend3DInf()
{
    
}

uint64_t CHmiSend3DInf::GetCurrentTimestampMs() 
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return static_cast<uint64_t>(milliseconds);
}

int32_t CHmiSend3DInf::SendHmi3dOutput(int sock,const Hmi_test_info& tHmi3dTestInfo)
{
    if (sock < 0)
    {
        return -1;
    }

    m_PB_Hmi3dOutputData.Clear();    

    //转换数据
    if(ConvertHmi3dOutputPB(tHmi3dTestInfo,&m_PB_Hmi3dOutputData))
    {
        printf("fail to ConvertHmi3dOutputPB\n");
        return -1;
    }

    // 序列化protobuf消息
    std::string serialized_data;
    
    if (!m_PB_Hmi3dOutputData.SerializeToString(&serialized_data)) {
        std::cerr << "Failed to serialize m_PB_Hmi3dOutputData." << std::endl;
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

void CHmiSend3DInf::AddPbBaseHead(uint32_t uHead,uint32_t uDataType,PB_Hmi3dOutputData* pb_hmi_3d_output)
{
    if(pb_hmi_3d_output == NULL)
    {
        printf("pb_hmi_3d_output is null\n");
        return ;
    }

    p_base_head = pb_hmi_3d_output->mutable_basehead();
    if(p_base_head != NULL)
    {
         p_base_head->set_crc_head(uHead);   
         p_base_head->set_data_type(uDataType);
    }
    else
    {
        printf("p_base_head is null\n");
    }

}

int32_t CHmiSend3DInf::ConvertHmi3dOutputPB(const Hmi_test_info& tHmi3dTestInfo, PB_Hmi3dOutputData* pb_hmi_3d_output) {
        
    if(pb_hmi_3d_output == NULL)
    {
        printf("pb_hmi_3d_output is null\n");
        return -1;
    }

    //add base head
    AddPbBaseHead(CRC_HEAD,HMI_3D_OUTPUT_PROTO,pb_hmi_3d_output);

    pb_hmi_3d_output->set_ltimestamp_ms(tHmi3dTestInfo.lTimestamp_ms);
    pb_hmi_3d_output->set_hmi_apa_sel(tHmi3dTestInfo.Hmi_apa_sel);
    pb_hmi_3d_output->set_hmi_park_mode(tHmi3dTestInfo.Hmi_park_mode);
    pb_hmi_3d_output->set_hmi_start_park(tHmi3dTestInfo.Hmi_start_park);
    pb_hmi_3d_output->set_hmi_cancel_req(tHmi3dTestInfo.Hmi_cancel_req);
    pb_hmi_3d_output->set_hmi_selected(tHmi3dTestInfo.Hmi_Selected);
    pb_hmi_3d_output->set_hmi_pause_req(tHmi3dTestInfo.Hmi_pause_req);
    pb_hmi_3d_output->set_hmi_resum_req(tHmi3dTestInfo.Hmi_resum_req);
    pb_hmi_3d_output->set_hmi_tragparkconfswt(tHmi3dTestInfo.Hmi_tragParkConfSwt);
    pb_hmi_3d_output->set_hmi_parkslot_id(tHmi3dTestInfo.Hmi_parkslot_id);
    pb_hmi_3d_output->set_hmi_parkout_dir(tHmi3dTestInfo.Hmi_parkout_dir);
    pb_hmi_3d_output->set_hmi_data_num(tHmi3dTestInfo.Hmi_data_num);
    for(int i = 0;i<8;i++)
    {
        pb_hmi_3d_output->add_hmi_slot_points(tHmi3dTestInfo.Hmi_slot_points[i]);
    }

    return 0;
}