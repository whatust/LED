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
		cin >> hex >> text >> key;

		cout << "TEXT: " << hex << text << endl;
		cout << "KEY: " << hex << key << endl;
		
		uint8_t *text_array;
		uint8_t *key_array;

		text_array = utin64touint8(text);
		key_array = utin64touint8(key);

		decryptBlock(text_array, key_array);

		cout << "Cypher text:" << endl;
		cout << hex << uint8touint64(text_array) << endl << endl;

		delete [] text_array;
		delete [] key_array;

	}else if(mode == 't'){
		testComp();
	}else{
		cout << ERR_MSG << endl;
	}

	return 0;
}
