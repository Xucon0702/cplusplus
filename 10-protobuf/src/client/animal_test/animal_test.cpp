#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "animal_test.h"


void sendAnimal(int sock, const Animal& animal) {
  std::string serialized_data;
  animal.SerializeToString(&serialized_data);

  if (send(sock, serialized_data.data(), serialized_data.size(), 0) == -1) {
    std::cerr << "Failed to send data" << std::endl;
    return;
  }
  std::cout << "Sent data successfully" << std::endl;
}

void animal_test(int sock)
{
  
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

  sendAnimal(sock, animal);
}
