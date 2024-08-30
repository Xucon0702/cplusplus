#include "hmi_3d_send.h"
#include <sys/types.h>
#include <sys/socket.h>

CHmi3DSendInf::CHmi3DSendInf()
{
    p_frame_header == NULL;
    p_UssSectorInfo == NULL;
    // p_frame_header == NULL;
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


int32_t CHmi3DSendInf::ConvertUssPdcToPB(const ZU2UssSectorOutputData_t& uss_pdc, PB_UssSectorOutputData* pb_uss_pdc) {
        
        if(pb_uss_pdc == NULL)
        {
            printf("pb_uss_pdc is null\n");
            return -1;
        }

        // 转换FrameHead
        p_frame_header = pb_uss_pdc->mutable_framehead();
        if(p_frame_header == NULL)
        {
            printf("p_frame_header is null\n");
            return -1;
        }
        p_frame_header->set_nframeid(uss_pdc.FrameHead.nFrameId);
        p_frame_header->set_nrervered(uss_pdc.FrameHead.nRervered);
        p_frame_header->set_itimemsec(uss_pdc.FrameHead.ITimeMsec);

        // 转换SectorData
        p_UssSectorInfo = pb_uss_pdc->mutable_sectordata();
        if(p_UssSectorInfo == NULL)
        {
            printf("p_UssSectorInfo is null\n");
            return -1;
        }
        for (int i = 0; i < 32; ++i) {
            p_UssSectorInfo->add_pdc_distance(uss_pdc.SectorData.PDC_Distance[i]);
            p_UssSectorInfo->add_laeb_distance(uss_pdc.SectorData.LAEB_Distance[i]);
        }

        for (int i = 0; i < 12; ++i) {
            p_UssSectorInfo->add_sensorstatus(uss_pdc.SectorData.SensorStatus[i]);
        }

        p_UssSectorInfo->set_systemstatus(uss_pdc.SectorData.SystemStatus);
        p_UssSectorInfo->set_nrervered(uss_pdc.SectorData.nRervered);

    return 0;
}