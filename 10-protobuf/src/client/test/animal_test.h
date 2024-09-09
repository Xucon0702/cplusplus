#ifndef ANIMAL_TEST_H_
#define ANIMAL_TEST_H_

#include "animal/animal.pb.h"

void sendAnimal(int sock, const Animal& animal);
void animal_test(int sock);

#endif