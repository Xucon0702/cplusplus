#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "test/recv_animal.h"


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