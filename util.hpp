#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <iomanip>

#define MASK_HIGH ((uint8_t)0xf0)
#define MASK_LOW  ((uint8_t)0x0f)

using namespace std;

void printBlock(uint8_t *block);
uint8_t *utin64touint8(uint64_t t);
uint64_t uint8touint64(uint8_t *a);

#endif