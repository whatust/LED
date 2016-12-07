#include "util.hpp"
#include "led.hpp"
#include "led_const.hpp"

void addConst(uint8_t *text, int round){

	uint8_t c;
	uint8_t k, r;
	uint8_t sB[8];

	c = RCONST[round];

	//Calculate the const matrix
	for(int i=0; i<4; ++i){

		if(i < 2)
			k = 0x4;
		else
			k = 0x0;

		if(i%2)
			r = c & 0x7;
		else
			r = c >> 3;

		sB[2*i] = (i^k) << 4;
		sB[2*i] |= r;
		sB[2*i+1] = 0 << 4;
		sB[2*i+1] |= 0;
	}

	//Apply const
	for(int i=0; i<8; ++i){
		text[i] ^= sB[i];
	}

	return;
}

void subCell(uint8_t *text){

	uint8_t b;
	uint8_t n1, n2;

	for(int i=0; i<8; ++i){
		b = text[i];
		n1 = b >> 4;
		n2 = b & MASK_LOW;

		if(n1%2)
			text[i] = (MASK_LOW & SBOX[n1/2]) << 4;
		else
			text[i] = MASK_HIGH & SBOX[n1/2];

		if(n2%2)
			text[i] |= MASK_LOW & SBOX[n2/2];
		else
			text[i] |= (MASK_HIGH & SBOX[n2/2]) >> 4;
	}

	return;
}

void shiftRow(uint8_t *text){

	uint16_t r;
	uint16_t s;

	for(int i=1; i<4; ++i){
		
		r = text[2*i] << 8;
		r |= text[2*i+1];

		s = r >> 4*(4-i);

		r <<= 4*i;

		r |= s;

		text[2*i] = (r & MASK_BH) >> 8;
		text[2*i+1] = r & MASK_BL;
	}

	return;
}

uint8_t multGF(uint8_t m, uint8_t a){

	switch(m){
		case 0x01:
			return a;
		case 0x02:
			return MULT2[a];
		case 0x04:
			return MULT2[MULT2[a]];
		case 0x05:
			return MULT2[MULT2[a]]^a;
		case 0x06:
			return MULT2[MULT2[a]^a];
		case 0x08:
			return MULT2[MULT2[MULT2[a]]];
		case 0x09:
			return MULT2[MULT2[MULT2[a]]]^a;
		case 0x0a:
			return MULT2[MULT2[MULT2[a]]^a];
		case 0x0b:
			return MULT2[MULT2[MULT2[a]]^a]^a;
		case 0x0e:
			return MULT2[MULT2[MULT2[a]]^MULT2[a]^a];
		case 0x0f:
			return MULT2[MULT2[MULT2[a]]^MULT2[a]^a]^a;
		default:
			cerr << "m Unexpected m value: " << ios::hex << static_cast<int>(m) << endl;
			exit(1);
	}

}

void mixColumn(uint8_t *text){

	uint8_t copy_text[8];
	uint8_t t_low, t_hi;
	uint8_t m_low, m_hi;

	for(int i=0; i<8; ++i){
		copy_text[i] = text[i];
	}

	//Col T
	for(int k=0; k<4; ++k){
		//Row M
		for(int i=0; i<4; ++i){
			
			uint8_t sum=0x00;

			//Col M
			for(int j=0; j<2; ++j){
				
				m_low = MASK_LOW & MIXCOL[(2*i)+j];
				m_hi = MIXCOL[(2*i)+j]>>4;

				if(k%2){
					t_hi = MASK_LOW & copy_text[4*j+k/2];
					t_low = MASK_LOW & copy_text[4*j+2+k/2];
				}else{
					t_hi = copy_text[4*j+k/2] >> 4;
					t_low = copy_text[4*j+2+k/2] >> 4;
				}

				sum ^= multGF(m_hi, t_hi);
				sum ^= multGF(m_low, t_low);
			}

			if(k%2)
				text[(2*i)+k/2] = (text[(2*i)+k/2] & MASK_HIGH) | sum;
			else
				text[(2*i)+k/2] = sum << 4;
		}
	}
	return;
}

void step(uint8_t *text, int round){
	
	for(int r=0; r<4; ++r){
		addConst(text, 4*round+r);
		//cout << "addConst" << endl;
		//printBlock(text);

		subCell(text);
		//cout << "SubCell" << endl;
		//printBlock(text);

		shiftRow(text);
		//cout << "shiftRow" << endl;
		//printBlock(text);

		mixColumn(text);
		//cout << "mixColumn" << endl;
		//printBlock(text);
	}

	return;
}

void addKey(uint8_t *text, uint8_t *key){

	for(int i=0; i<8; ++i){
		text[i] ^= key[i];
	}

	return;
}

void encryptBlock(uint8_t *text, uint8_t *key){

	clock_t time; 
	time = clock();

	for(int s=0; s < 8; ++s){
		addKey(text, key);
		step(text, s);
	}
	addKey(text, key);

	time = clock()-time;
	cout << "clicks: " <<(double) time << endl;

	return;
}

void decryptBlock(uint8_t *text, uint8_t *key){

	clock_t time; 
	time = clock();

	for(int s=0; s < 7; ++s){
		addKey(text, key);
		//srtep(text, s);
	}
	addKey(text, key);

	time = clock()-time;
	cout << "clicks: " <<(double) time << endl;

	return;
}
