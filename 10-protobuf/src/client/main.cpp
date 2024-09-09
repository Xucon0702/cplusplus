#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#include "test/animal_test.h"
#include "hmi_3d_transfer/hmi_3d_send.h"
#include "hmi_3d_transfer/hmi_3d_recv.h"

CHmi3DSendInf g_sendInf;
CRecvHmi3D   g_recv3d;
int g_sock;
void StartRecvThread()
{
    while(1)
    {
        g_recv3d.RecvHmi3dOutput(g_sock,&(g_recv3d.m_PB_Hmi3dOutputData));
        usleep(40*100);
    }
}

void SetSendDataThread()
{
  ApaStateToHmiTestInfo tApaStateToHmiTestInfo;
  MvCanCarInfo tMvCanCarInfo;
  ApaAvapSlotOut tApaAvapSlotOut;
  MvApaAvapObjOut tMvApaAvapObjOut;
  ZU2UssSectorOutputData_t tZU2UssSectorOutputData_t;
  ApaPlanTrackInfo tApaPlanTrackInfo;
  PlanFullPath tPlanFullPath;

  uint32_t tmpNum = 1;

  while(1)
  {
    memset(&tApaStateToHmiTestInfo,0,sizeof(tApaStateToHmiTestInfo));
    memset(&tMvCanCarInfo,0,sizeof(tMvCanCarInfo));
    memset(&tApaAvapSlotOut,0,sizeof(tApaAvapSlotOut));
    memset(&tMvApaAvapObjOut,0,sizeof(tMvApaAvapObjOut));
    memset(&tZU2UssSectorOutputData_t,0,sizeof(tZU2UssSectorOutputData_t));
    memset(&tApaPlanTrackInfo,0,sizeof(tApaPlanTrackInfo));
    memset(&tPlanFullPath,0,sizeof(tPlanFullPath));

    tApaStateToHmiTestInfo.lTimestampMs = tmpNum;
    tApaStateToHmiTestInfo.AVAP_APAFunDispSts = tmpNum+1;

    tMvCanCarInfo.lTimeMsec = tmpNum+2;
    tMvCanCarInfo.ExternalHumidityValidity = tmpNum+3;

    tApaAvapSlotOut.TimeMsec = tmpNum+4;
    tApaAvapSlotOut.PrkgSlot[0].APA_PrkgSlot_Sts = 0x2;
    tApaAvapSlotOut.PrkgSlot[0].APA_SlotACoorX = 76;
    tApaAvapSlotOut.PrkgSlot[0].APA_SlotACoorY = 133;
    tApaAvapSlotOut.PrkgSlot[0].APA_SlotBCoorX = 16;
    tApaAvapSlotOut.PrkgSlot[0].APA_SlotBCoorY = 132;
    tApaAvapSlotOut.PrkgSlot[0].APA_SlotCCoorX = 16;
    tApaAvapSlotOut.PrkgSlot[0].APA_SlotCCoorY = 111;
    tApaAvapSlotOut.PrkgSlot[0].APA_SlotDCoorX = 76;
    tApaAvapSlotOut.PrkgSlot[0].APA_SlotDCoorY = 112;

    tMvApaAvapObjOut.TimeMsec = tmpNum+5;


    tZU2UssSectorOutputData_t.FrameHead.nFrameId = tmpNum+6;
    tZU2UssSectorOutputData_t.FrameHead.ITimeMsec = tmpNum+7;
    for(int i=0; i<12; i++)
    {
      tZU2UssSectorOutputData_t.SectorData.LAEB_Distance[i] = i+1000;
      tZU2UssSectorOutputData_t.SectorData.SensorStatus[i] = 1;
    }
    

    tApaPlanTrackInfo.lTimeMsec = tmpNum+8;
    tApaPlanTrackInfo.tPlanToHdmi.uCmd = 1;
    tApaPlanTrackInfo.tPlanToHdmi.uTotalStep = 3;
    tApaPlanTrackInfo.tPlanToHdmi.uCurrentStep = 2;
    tApaPlanTrackInfo.tPlanToHdmi.uDisStep = 15;
    tApaPlanTrackInfo.tPlanToHdmi.px[0] = 1.;
    tApaPlanTrackInfo.tPlanToHdmi.px[1] = 2.;
    tApaPlanTrackInfo.tPlanToHdmi.px[2] = 3.;
    tApaPlanTrackInfo.tPlanToHdmi.px[3] = 4.;
    tApaPlanTrackInfo.tPlanToHdmi.py[0] = 1.;
    tApaPlanTrackInfo.tPlanToHdmi.py[1] = 2.;
    tApaPlanTrackInfo.tPlanToHdmi.py[2] = 3.;
    tApaPlanTrackInfo.tPlanToHdmi.py[3] = 4.;

    tApaPlanTrackInfo.validNum = 50;
    for(int i = 0; i < tApaPlanTrackInfo.validNum; i++)
    {
      tApaPlanTrackInfo.aTracksPlan[i].point.x = i;
      tApaPlanTrackInfo.aTracksPlan[i].point.y = i;
    }


    tPlanFullPath.lTimeMsec = tmpNum+9;
    tPlanFullPath.validNum = 3;
    for(int i = 0; i < tPlanFullPath.validNum; i++)
    {
      tPlanFullPath.Array_APAPathPoints[i].validNum = 50+i;
      for(int j = 0; j < tPlanFullPath.Array_APAPathPoints[i].validNum; j++)
      {
        tPlanFullPath.Array_APAPathPoints[i].aPoints[j].fX = j;
        tPlanFullPath.Array_APAPathPoints[i].aPoints[j].fY = j;
      }
    }

    g_sendInf.SetHmi3DApaStateBuf(&tApaStateToHmiTestInfo);
    g_sendInf.SetHmi3DCanBuf(&tMvCanCarInfo);
    g_sendInf.SetHmi3DSlotBuf(&tApaAvapSlotOut);
    g_sendInf.SetHmi3DOdBuf(&tMvApaAvapObjOut);
    g_sendInf.SetHmi3DUssPdcBuf(&tZU2UssSectorOutputData_t);
    g_sendInf.SetHmi3DPlanTrackBuf(&tApaPlanTrackInfo);
    g_sendInf.SetHmi3DPlanFullPathBuf(&tPlanFullPath);

    tmpNum++;
    usleep(1000*100);
  }
}

int main() {


  // 创建 socket 连接
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    std::cerr << "Failed to create socket" << std::endl;
    return 1;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(HMI_3D_PORT);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    std::cerr << "Failed to connect to server" << std::endl;
    return 1;
  }
 // 发送数据    quaternion_test(sock);        
//   sendAnimal(sock, animal);
    // animal_test(sock);
    
    ZU2UssSectorOutputData_t uss_pdc = {
        {1, 0, 123456789}, // FrameHead
        { // SectorData
            {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 320},
            {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 320},
            {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
            1,
            0
        }
    };

    
    // int result = transfer.SendUssPdc(sock, uss_pdc);
   

  g_sock = sock;
  //开启接收线程
  std::thread recv_3d_thread(StartRecvThread);
  //开启发送线程
  std::thread set_3d_thread(SetSendDataThread);
  while(1)
  {
     int result = g_sendInf.SendHmi3dPackage(sock);
    // int result = transfer.SendHmi3dPackage(sock, uss_pdc);
  
    if (result > 0) {
        std::cout << "Sent " << result << " bytes." << std::endl;
    } else {
        std::cerr << "Failed to send data." << std::endl;
    }

    sleep(2);
  }

  // 关闭连接
  close(sock);
  recv_3d_thread.detach();
  set_3d_thread.detach();

  return 0;
}