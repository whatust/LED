#include "util.hpp"

void printBlock(uint8_t *block){

	for(int i=0; i < 8; ++i){
		
		if(i%2 == 0 && i > 0){
			cout << endl;
		}

		if((i+1)%2 == 0 && i > 0){
			cout << " ";
		}

		cout << hex << static_cast<int>(block[i]>>4);
		cout << hex << static_cast<int>(block[i]&MASK_LOW);

	}
	cout << endl;
}

uint8_t *utin64touint8(uint64_t t){

	uint8_t b;
	uint8_t *a = new uint8_t[8];
	uint64_t m = 0xff00000000000000;

	for(int i=0; i<8; ++i){
		b = (m&t) >> (7-i)*8;
		m >>= 8;

		a[i] = (uint8_t)b;
	}
	return a;
}

uint64_t uint8touint64(uint8_t *a){

	uint64_t b=0;

	for(int i=0; i<8; ++i){
		b <<= 8;
		b |= a[i];
	}
	return b;
}