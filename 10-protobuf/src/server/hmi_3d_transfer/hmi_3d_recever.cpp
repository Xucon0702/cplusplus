#include "hmi_3d_recever.h"

#include <iostream>
#include <string>
#include <vector>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#include <sys/types.h>
#include <sys/socket.h>


CHmi3DReceiver::CHmi3DReceiver()
{

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

    // 打印解析结果
    PrintPbHmi3dPackage(*pb_hmi_3d_package);

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