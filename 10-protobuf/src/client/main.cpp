#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "animal_test/animal_test.h"
#include "hmi_3d_transfer/hmi_3d_send.h"
 
int main() {


  // 创建 socket 连接
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    std::cerr << "Failed to create socket" << std::endl;
    return 1;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(9999);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    std::cerr << "Failed to connect to server" << std::endl;
    return 1;
  }

  // 发送数据
//   sendAnimal(sock, animal);
    animal_test(sock);
    
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

    CHmi3DSendInf transfer;

    int result = transfer.SendUssPdc(sock, uss_pdc);

    if (result > 0) {
        std::cout << "Sent " << result << " bytes." << std::endl;
    } else {
        std::cerr << "Failed to send data." << std::endl;
    }

  // 关闭连接
  close(sock);

  return 0;
}