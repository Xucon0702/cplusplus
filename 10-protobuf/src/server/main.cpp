#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "test/recv_animal.h"

#include "hmi_3d_transfer/hmi_3d_recever.h"

int main() {
  // 创建 socket 服务器
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    std::cerr << "Failed to create socket" << std::endl;
    return 1;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(9999);
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
  CHmi3DReceiver receiver_3d;
  PB_UssSectorOutputData PB_uss_pdc;

  while (true) {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket == -1) {
      std::cerr << "Failed to accept connection" << std::endl;
      continue;
    }

    std::cout << "Connected by " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

    Animal animal;
    receiveAnimal(client_socket, &animal);

    receiver_3d.ReceiveUssPdc(client_socket,&PB_uss_pdc);

    // 关闭连接
    close(client_socket);
  }

  // 关闭服务器套接字
  close(server_socket);

  return 0;
}