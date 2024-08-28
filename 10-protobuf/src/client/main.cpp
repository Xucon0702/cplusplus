#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "animal/animal.pb.h"

void sendAnimal(int sock, const Animal& animal) {
  std::string serialized_data;
  animal.SerializeToString(&serialized_data);

  if (send(sock, serialized_data.data(), serialized_data.size(), 0) == -1) {
    std::cerr << "Failed to send data" << std::endl;
    return;
  }
  std::cout << "Sent data successfully" << std::endl;
}

int main() {
  // 创建 Person 对象
  Person person;
  person.set_name("Alice");
  person.set_age(30);

  // 创建 Dog 对象
  Dog dog1;
  dog1.set_name("Buddy");
  dog1.set_age(5);
  dog1.set_type(DogType::POODLE);

  Dog dog2;
  dog2.set_name("Max");
  dog2.set_age(3);
  dog2.set_type(DogType::GOLDEN);

  // 创建 Animal 对象
  Animal animal;
  *animal.mutable_human() = person;
  animal.add_dog()->CopyFrom(dog1);
  animal.add_dog()->CopyFrom(dog2);

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
  sendAnimal(sock, animal);

  // 关闭连接
  close(sock);

  return 0;
}