#include "util.hpp"

#ifndef _LED_H_
#define _LED_H_

void encryptBlock(uint8_t *text, uint8_t *key);
void decryptBlock(uint8_t *text, uint8_t *key);

void addConst(uint8_t *text, int round);
void subCell(uint8_t *text);
void shiftRow(uint8_t *text);
void mixColumn(uint8_t *text);

#endif