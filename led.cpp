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

void subCell(uint8_t *text, uint8_t *s){

	uint8_t b;
	uint8_t n1, n2;

	for(int i=0; i<8; ++i){
		b = text[i];
		n1 = b >> 4;
		n2 = b & MASK_LOW;

		if(n1%2)
			text[i] = (MASK_LOW & s[n1/2]) << 4;
		else
			text[i] = MASK_HIGH & s[n1/2];

		if(n2%2)
			text[i] |= MASK_LOW & s[n2/2];
		else
			text[i] |= (MASK_HIGH & s[n2/2]) >> 4;
	}

	return;
}

uint16_t shift(uint16_t r, int i){
	
	uint16_t s;

	s = r >> 4*(4-i);
	r <<= 4*i;
	r |= s;

	return r;
}

void shiftRow(uint8_t *t){

	uint16_t r;

	for(int i=1; i<4; ++i){

		r = t[2*i] << 8;
		r |= t[2*i+1];

		r = shift(r, i);

		t[2*i] = (r & MASK_BH) >> 8;
		t[2*i+1] = r & MASK_BL;
	}

	return;
}

void rshiftRow(uint8_t *t){

	uint16_t r;

	for(int i=1; i<4; ++i){

		r = t[2*i] << 8;
		r |= t[2*i+1];

		r = shift(r, 4-i);

		t[2*i] = (r & MASK_BH) >> 8;
		t[2*i+1] = r & MASK_BL;
	}

	return;
}

uint8_t multGF(uint8_t m, uint8_t a){

	switch(m){
		case 0x00:
			return 0;
		case 0x01:
			return a;
		case 0x02:
			return MULT2[a];
		case 0x03:
			return MULT2[a]^a;
		case 0x04:
			return MULT2[MULT2[a]];
		case 0x05:
			return MULT2[MULT2[a]]^a;
		case 0x06:
			return MULT2[MULT2[a]^a];
		case 0x07:
			return MULT2[MULT2[a]]^MULT2[a]^a;
		case 0x08:
			return MULT2[MULT2[MULT2[a]]];
		case 0x09:
			return MULT2[MULT2[MULT2[a]]]^a;
		case 0x0a:
			return MULT2[MULT2[MULT2[a]]^a];
		case 0x0b:
			return MULT2[MULT2[MULT2[a]]^a]^a;
		case 0x0c:
			return MULT2[MULT2[MULT2[a]^a]];
		case 0x0d:
			return MULT2[MULT2[MULT2[a]^a]]^a; 
		case 0x0e:
			return MULT2[MULT2[MULT2[a]]^MULT2[a]^a];
		case 0x0f:
			return MULT2[MULT2[MULT2[a]]^MULT2[a]^a]^a;
		default:
			cerr << "m Unexpected m value: " << ios::hex << static_cast<int>(m) << endl;
			exit(1);
	}
}

void mixColumn(uint8_t *text, uint8_t *m){

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
				
				m_low = MASK_LOW & m[(2*i)+j];
				m_hi = m[(2*i)+j]>>4;

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

		subCell(text, SBOX);
		//cout << "SubCell" << endl;
		//printBlock(text);

		shiftRow(text);
		//cout << "shiftRow" << endl;
		//printBlock(text);

		mixColumn(text, MIXCOL);
		//cout << "mixColumn" << endl;
		//printBlock(text);
	}

	return;
}

void rstep(uint8_t *text, int round){
	
	for(int r=0; r<4; ++r){
		mixColumn(text, INV_MIXCOL);
		//cout << "mixColumn" << endl;
		//printBlock(text);

		rshiftRow(text);
		//cout << "shiftRow" << endl;
		//printBlock(text);

		subCell(text, INV_SBOX);
		//cout << "SubCell" << endl;
		//printBlock(text);

		addConst(text, 31-(4*round+r));
		//cout << "addConst" << endl;
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

double encryptBlock(uint8_t *text, uint8_t *key){

	double c;
	c = click();

	for(int s=0; s < 8; ++s){
		addKey(text, key);
		step(text, s);
	}
	addKey(text, key);

	c = click()-c;
	//cout << "clocks: " << (double) c << endl;

	return c;
}

double decryptBlock(uint8_t *text, uint8_t *key){

	clock_t time; 
	double c;
	time = clock();
	c = click();

	for(int s=0; s < 8; ++s){
		addKey(text, key);
		rstep(text, s);
	}
	addKey(text, key);

	c = click()-c;
	time = clock()-time;
	//cout << "clocks: " << (double) c << endl;

	return c;
}

void testTime(){

	double sumEncTime=0.0;
	double sumDecTime=0.0;

	uint64_t t, k;
	uint8_t *t_array;
	uint8_t *k_array;

	for(int i=0; i<NSAMPLES; ++i){

		syscall(SYS_getrandom, &t, 8, GRND_NONBLOCK);
		syscall(SYS_getrandom, &k, 8, GRND_NONBLOCK);

		t_array = utin64touint8(t);
		k_array = utin64touint8(k);

		sumEncTime += encryptBlock(t_array, k_array);
		sumDecTime += decryptBlock(t_array, k_array);

	}
	cout << "average encrypt (clock/byte): " << sumEncTime/(NSAMPLES*8) << endl;
	cout << "average decrypt (clock/byte): " << sumDecTime/(NSAMPLES*8) << endl;

	return;
}

void testComp(){

	uint64_t shift = 0x0123456789abcdef;
	uint8_t * shift_array;

	shift_array = utin64touint8(shift);
	cout << "Shift Test:" << endl;
	printBlock(shift_array);
	cout << "Shift Result:" << endl;
	shiftRow(shift_array);
	printBlock(shift_array);
	delete [] shift_array;

	uint64_t rshift = 0x01235674ab89fcde;
	uint8_t * rshift_array;

	rshift_array = utin64touint8(rshift);
	cout << "rShift Test:" << endl;
	printBlock(rshift_array);
	cout << "rShift Result:" << endl;
	rshiftRow(rshift_array);
	printBlock(rshift_array);
	delete [] rshift_array;

	uint64_t addcon = 0x0000000000000000;
	uint8_t *addcon_array;

	addcon_array = utin64touint8(addcon);
	cout << endl << "Add Constante Test:" << endl;
	printBlock(addcon_array);
	cout << "Add Constante Result:" << endl;
	addConst(addcon_array,4);
	printBlock(addcon_array);
	delete [] addcon_array;

	uint64_t sub = 0x0123456789abcdef;
	uint8_t * sub_array;

	sub_array = utin64touint8(sub);
	cout << endl << "Sub Test:" << endl;
	printBlock(sub_array);
	cout << "Sub Result:" << endl;
	subCell(sub_array, SBOX);
	printBlock(sub_array);
	delete [] sub_array;

	uint64_t rsub = 0xc56b90ad3ef84712;
	uint8_t *rsub_array;

	rsub_array = utin64touint8(rsub);
	cout << "rSub Test:" << endl;
	printBlock(rsub_array);
	cout << "rSub Result:" << endl;
	subCell(rsub_array, INV_SBOX);
	printBlock(rsub_array);
	delete [] rsub_array;

	uint64_t mix = 0xccd43845762ed99d;
	uint8_t *mix_array;

	mix_array = utin64touint8(mix);
	cout << endl << "Mix Test:" << endl;
	printBlock(mix_array);
	cout << "Mix Result:" << endl;
	mixColumn(mix_array, MIXCOL);
	printBlock(mix_array);
	delete [] mix_array;

	uint64_t rmix = 0x41228656bea922fb;
	uint8_t *rmix_array;

	rmix_array = utin64touint8(rmix);
	cout << "rMix Test:" << endl;
	printBlock(rmix_array);
	cout << "rMix Result:" << endl;
	mixColumn(rmix_array, INV_MIXCOL);
	printBlock(rmix_array);
	delete [] rmix_array;
}