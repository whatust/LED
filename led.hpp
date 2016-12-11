//William Tustumi

#include "util.hpp"
#include <unistd.h>
#include <syscall.h>
#include <linux/random.h>

#ifndef _LED_H_
#define _LED_H_

#define NSAMPLES 50000

double encryptBlock(uint8_t *text, uint8_t *key);
double decryptBlock(uint8_t *text, uint8_t *key);

void addConst(uint8_t *text, int round);
void subCell(uint8_t *text, uint8_t *s);
void shiftRow(uint8_t *text);
void mixColumn(uint8_t *text, uint8_t *m);
void testComp();
void testTime();

#endif