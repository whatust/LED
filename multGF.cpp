//William Tustumi

#include <iostream>
#define MASK_LOW 0x0f

int MULT2[16] = {0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x03, 0x01, 0x07, 0x05, 0x0B, 0x09, 0x0F, 0x0D};

using namespace std;


int multGF(int m, int a){

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

void multRow(int m, int *r){

	for(int i=0; i<4; ++i){
		int l = r[i] & MASK_LOW;

		cout << hex << static_cast<int>(multGF(m, l)) << " ";	
	}
	cout << endl;
	return;
}

void sumRow(int *r, int *t){

	for(int i=0; i<4; ++i){
		int l = r[i] & MASK_LOW;

		int lt = t[i] & MASK_LOW;
		
		cout << hex << static_cast<int>(l^lt) << " ";	
	}
	cout << endl;
	return;
}

int main (){

	char c;
	int r[4];

	cin >> c;

	while(c != 'q'){

		cout << "enter row:";

		for(int i=0; i<4; ++i){
			cin >> hex >> r[i];
		}

		if(c == 'm'){

			cout << "enter int:";

			int m;
			cin >> m;

			multRow(m, r);

		}else if(c == 's'){

			cout << "enter second row:";

			int t[2];
			for(int i=0; i<4; ++i){
				cin >> hex >> t[i];
			}

			sumRow(r, t);
		}
		cin >> c;
	}
	return 0;
}