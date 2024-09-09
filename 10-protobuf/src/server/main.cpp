#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "test/recv_animal.h"

#include "hmi_3d_transfer/hmi_3d_recever.h"
#include "hmi_3d_transfer/hmi_send_3d.h"

// CHmi3DSendInf g_sendInf;
// CRecvHmi3D   g_recv3d;
CHmi3DReceiver g_receiver_3d;
CHmiSend3DInf  g_send_3d;
int g_sock;
void StartRecvThread()
{
  printf("StartRecvThread 1\n");
  while(1)
  {
    printf("StartRecvThread\n");
    g_receiver_3d.RecvHmi3dPackage(g_sock,&(g_receiver_3d.m_PB_Hmi3dPackage));

    

    usleep(40*100);
  }
}

void StartSendDataThread()
{
  printf("StartSendDataThread 1\n");
  Hmi_test_info tHmi_test_info;
  uint32_t runCnt = 1;

  while(1)
  {
      memset(&tHmi_test_info,0,sizeof(Hmi_test_info));

      tHmi_test_info.lTimestamp_ms = g_send_3d.GetCurrentTimestampMs();
      tHmi_test_info.Hmi_apa_sel = runCnt+1;
      tHmi_test_info.Hmi_park_mode = runCnt+2;
      tHmi_test_info.Hmi_start_park = runCnt+3;
      tHmi_test_info.Hmi_cancel_req = runCnt+4;
      tHmi_test_info.Hmi_Selected = runCnt+5;
      tHmi_test_info.Hmi_pause_req = runCnt+6;
      tHmi_test_info.Hmi_resum_req = runCnt+7;
      tHmi_test_info.Hmi_tragParkConfSwt = runCnt+8;
      tHmi_test_info.Hmi_parkslot_id = runCnt+9;
      tHmi_test_info.Hmi_parkout_dir = runCnt+10;
      tHmi_test_info.Hmi_data_num = runCnt+11;

      for(int i = 0;i<8;i++)
      {
        tHmi_test_info.Hmi_slot_points[i] = runCnt+i;
      }

      g_send_3d.SendHmi3dOutput(g_sock,tHmi_test_info);

      runCnt++;
      usleep(1000*1000);
      
  }
}

int main() {
  // 创建 socket 服务器
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    std::cerr << "Failed to create socket" << std::endl;
    return 1;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(HMI_3D_PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    std::cerr << "Failed to bind socket" << std::endl;
    return 1;
  }

  if (listen(server_socket, 5) == -1) {
    std::cerr << "Failed to listen on socket" << std::endl;
    return 1;
  }

  std::cout << "Waiting for connection..." << std::endl;

  //hmi-3d接收
  PB_UssSectorOutputData PB_uss_pdc;

  int client_socket = 0;
  while (true) {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket == -1) {
      std::cerr << "Failed to accept connection" << std::endl;
      continue;
    }

    std::cout << "Connected by " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

    // Animal animal;
    // receiveAnimal(client_socket, &animal);

    // receiver_3d.ReceiveUssPdc(client_socket,&PB_uss_pdc);    
    break;
  }

  g_sock = client_socket;
  //开启接收线程
  std::thread recv_3d_thread(StartRecvThread);
  //开启发送线程
  std::thread send_3d_thread(StartSendDataThread);

  while(1)
  {
    printf("main task running...\n");
    usleep(2000*1000);
  }

  recv_3d_thread.detach();
  send_3d_thread.detach();

  // 关闭连接
  close(client_socket);
  // 关闭服务器套接字
  close(server_socket);

  return 0;
}