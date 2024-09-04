#include "hmi_3d_recv.h"

#include <iostream>
#include <string>
#include <vector>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#include <sys/types.h>
#include <sys/socket.h>


CRecvHmi3D::CRecvHmi3D()
{

}

CRecvHmi3D::~CRecvHmi3D()
{
    
}

int32_t CRecvHmi3D::RecvHmi3dOutput(int sock, PB_Hmi3dOutputData* pb_hmi_3d_output) {
    if(pb_hmi_3d_output == NULL)
    {
        std::cerr << "pb_hmi_3d_output is null" << std::endl;
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
    if (!pb_hmi_3d_output->ParseFromArray(buffer, bytes_received)) {
        std::cerr << "Failed to parse pb_hmi_3d_output." << std::endl;
        return -1;
    }

    // 打印解析结果
    PrintHmi3DOutput(pb_hmi_3d_output);

    //转为内部结构
    ConvertToHmiTestInfo(pb_hmi_3d_output,&m_Hmi_test_info);
    //冰羚发送
    // SendHmiTestInfo(&m_Hmi_test_info);

    return bytes_received;
}
void CRecvHmi3D::PrintHmi3DOutput(const PB_Hmi3dOutputData* pb_hmi_3d_output) 
{
    if (pb_hmi_3d_output == nullptr) {
            std::cerr << "PB_Hmi3dOutputData is null." << std::endl;
            return;
        }

        printf("PB_Hmi3dOutputData:\n");
        printf("  lTimestamp_ms: %lu\n", pb_hmi_3d_output->ltimestamp_ms());
        printf("  Hmi_apa_sel: %u\n", pb_hmi_3d_output->hmi_apa_sel());
        printf("  Hmi_park_mode: %u\n", pb_hmi_3d_output->hmi_park_mode());
        printf("  Hmi_start_park: %u\n", pb_hmi_3d_output->hmi_start_park());
        printf("  Hmi_cancel_req: %u\n", pb_hmi_3d_output->hmi_cancel_req());
        printf("  Hmi_Selected: %u\n", pb_hmi_3d_output->hmi_selected());
        printf("  Hmi_pause_req: %u\n", pb_hmi_3d_output->hmi_pause_req());
        printf("  Hmi_resum_req: %u\n", pb_hmi_3d_output->hmi_resum_req());
        printf("  Hmi_tragParkConfSwt: %u\n", pb_hmi_3d_output->hmi_tragparkconfswt());
        printf("  Hmi_parkslot_id: %u\n", pb_hmi_3d_output->hmi_parkslot_id());
        printf("  Hmi_parkout_dir: %u\n", pb_hmi_3d_output->hmi_parkout_dir());
        printf("  Hmi_slot_points: ");
        for (int i = 0; i < pb_hmi_3d_output->hmi_slot_points_size(); i++) {
            printf("%f ", pb_hmi_3d_output->hmi_slot_points(i));
        }
        printf("\n");
    
}

void CRecvHmi3D::ConvertToHmiTestInfo(const PB_Hmi3dOutputData* pb_hmi_3d_output,Hmi_test_info* pDest) 
{
    if (pb_hmi_3d_output == nullptr || pDest == nullptr) {
            printf("ConvertToHmiTestInfo param is null\n");
            return;
        }

        memset(pDest, 0, sizeof(Hmi_test_info));
        pDest->lTimestamp_ms = pb_hmi_3d_output->ltimestamp_ms();
        pDest->Hmi_apa_sel = pb_hmi_3d_output->hmi_apa_sel();
        pDest->Hmi_park_mode = pb_hmi_3d_output->hmi_park_mode();
        pDest->Hmi_start_park = pb_hmi_3d_output->hmi_start_park();
        pDest->Hmi_cancel_req = pb_hmi_3d_output->hmi_cancel_req();
        pDest->Hmi_pause_req = pb_hmi_3d_output->hmi_pause_req();
        pDest->Hmi_resum_req = pb_hmi_3d_output->hmi_resum_req();
        pDest->Hmi_tragParkConfSwt = pb_hmi_3d_output->hmi_tragparkconfswt();
        pDest->Hmi_parkslot_id = pb_hmi_3d_output->hmi_parkslot_id();
        pDest->Hmi_parkout_dir = pb_hmi_3d_output->hmi_parkout_dir();
        for (int i = 0; i < pb_hmi_3d_output->hmi_slot_points_size(); i++) {
            pDest->Hmi_slot_points[i] = pb_hmi_3d_output->hmi_slot_points(i);
        }

        // printf("PB_Hmi3dOutputData:\n");
        // printf("  lTimestamp_ms: %lu\n", pb_hmi_3d_output->ltimestamp_ms());
        // printf("  Hmi_apa_sel: %u\n", pb_hmi_3d_output->hmi_apa_sel());
        // printf("  Hmi_park_mode: %u\n", pb_hmi_3d_output->hmi_park_mode());
        // printf("  Hmi_start_park: %u\n", pb_hmi_3d_output->hmi_start_park());
        // printf("  Hmi_cancel_req: %u\n", pb_hmi_3d_output->hmi_cancel_req());
        // printf("  Hmi_Selected: %u\n", pb_hmi_3d_output->hmi_selected());
        // printf("  Hmi_pause_req: %u\n", pb_hmi_3d_output->hmi_pause_req());
        // printf("  Hmi_resum_req: %u\n", pb_hmi_3d_output->hmi_resum_req());
        // printf("  Hmi_tragParkConfSwt: %u\n", pb_hmi_3d_output->hmi_tragparkconfswt());
        // printf("  Hmi_parkslot_id: %u\n", pb_hmi_3d_output->hmi_parkslot_id());
        // printf("  Hmi_parkout_dir: %u\n", pb_hmi_3d_output->hmi_parkout_dir());
        // printf("  Hmi_slot_points: ");
        // for (int i = 0; i < pb_hmi_3d_output->hmi_slot_points_size(); i++) {
        //     printf("%f ", pb_hmi_3d_output->hmi_slot_points(i));
        // }
        // printf("\n");
    
}