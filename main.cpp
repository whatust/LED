//William Tustumi

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

		double clock = encryptBlock(text_array, key_array);

		cout << "Cypher text:" << endl;
		cout << hex << uint8touint64(text_array) << endl;

		cout << "Number of clocks: " << clock << endl << endl;

		delete [] text_array;
		delete [] key_array;

	}else if(mode == 'd'){
		cin >> hex >> text >> key;

		cout << "TEXT: " << hex << text << endl;
		cout << "KEY: " << hex << key << endl;
		
		uint8_t *text_array;
		uint8_t *key_array;

		text_array = utin64touint8(text);
		key_array = utin64touint8(key);

		double clock = decryptBlock(text_array, key_array);

		cout << "Cypher text:" << endl;
		cout << hex << uint8touint64(text_array) << endl;

		cout << "Number of clocks: " << clock << endl << endl;

		delete [] text_array;
		delete [] key_array;

	}else if(mode == 'c'){
		testComp();
	}else if(mode == 't'){
		testTime();
	}else{
		cout << ERR_MSG << endl;
	}

	return 0;
}
