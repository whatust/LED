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

#ifdef __i386__

static inline uint64_t click(){
	uint64_t t;
	__asm__ volitile (".byte 0x0f, 0x31" : "=A" (t));
	return t;
}
#endif

#ifdef __x86_64__

static inline uint64_t click(){
    uint32_t h, l;
    __asm__ __volatile__ ("rdtsc" : "=a"(l), "=d"(h));
    return ( (uint64_t)l)|( ((uint64_t)h)<<32 );
}
#endif

#endif