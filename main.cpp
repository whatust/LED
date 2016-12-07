#include "main.hpp"

int main (){
	
	char mode;
	uint64_t text;
	uint64_t key;

	cin >> mode;

	if(mode == 'e'){
		cin >> hex >> text >> key;
		cout << "TEXT: " << hex << text << endl;
		cout << "KEY: " << hex << key << endl;

		uint8_t *text_array;
		uint8_t *key_array;

		text_array = utin64touint8(text);
		key_array = utin64touint8(key);

		encryptBlock(text_array, key_array);

		cout << "Cypher text:" << endl;
		cout << hex << uint8touint64(text_array) << endl << endl;

		delete [] text_array;
		delete [] key_array;

	}else if(mode == 'd'){
		cin >> text >> key;

		uint8_t *text_array;
		uint8_t *key_array;

		text_array = utin64touint8(text);
		key_array = utin64touint8(key);

		encryptBlock(text_array, key_array);

		cout << "Plain text:" << endl;
		printBlock(text_array);

		delete [] text_array;
		delete [] key_array;

	}else if(mode == 't'){

		uint64_t shift = 0x0123456789abcdef;
		uint8_t * shift_array;

		shift_array = utin64touint8(shift);
		cout << "Shift Test:" << endl;
		printBlock(shift_array);
		cout << "Shift Result:" << endl;
		shiftRow(shift_array);
		printBlock(shift_array);
		delete [] shift_array;

		uint64_t addcon = 0x0000000000000000;
		uint8_t * addcon_array;

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
		subCell(sub_array);
		printBlock(sub_array);
		delete [] sub_array;

		uint64_t mix = 0x2000020000200002;
		uint8_t * mix_array;

		mix_array = utin64touint8(mix);
		cout << endl << "Mix Test:" << endl;
		printBlock(mix_array);
		cout << "Mix Result:" << endl;
		mixColumn(mix_array);
		printBlock(mix_array);
		delete [] mix_array;
	}else{
		cout << ERR_MSG << endl;
	}

	return 0;
}
