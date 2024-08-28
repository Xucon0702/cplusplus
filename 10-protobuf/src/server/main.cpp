#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "animal/animal.pb.h"

void receiveAnimal(int sock, Animal* animal) {
  char buffer[1024];
  ssize_t bytes_received = recv(sock, buffer, sizeof(buffer), 0);
  if (bytes_received == -1) {
    std::cerr << "Failed to receive data" << std::endl;
    return;
  }

  if (!animal->ParseFromArray(buffer, bytes_received)) {
    std::cerr << "Failed to parse data" << std::endl;
    return;
  }

  std::cout << "Received data:" << std::endl;
  std::cout << "Human Name: " << animal->human().name() << std::endl;
  std::cout << "Human Age: " << animal->human().age() << std::endl;
  for (int i = 0; i < animal->dog_size(); ++i) {
    const Dog& dog = animal->dog(i);
    std::cout << "Dog Name: " << dog.name() << std::endl;
    std::cout << "Dog Age: " << dog.age() << std::endl;
    std::cout << "Dog Type: ";
    switch (dog.type()) {
      case DogType::POODLE:
        std::cout << "POODLE" << std::endl;
        break;
      case DogType::SHEPHERD:
        std::cout << "SHEPHERD" << std::endl;
        break;
      case DogType::GOLDEN:
        std::cout << "GOLDEN" << std::endl;
        break;
      default:
        std::cout << "Unknown" << std::endl;
        break;
    }
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

    // 关闭连接
    close(client_socket);
  }

  // 关闭服务器套接字
  close(server_socket);

  return 0;
}