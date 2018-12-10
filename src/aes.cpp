#include<iostream>
#include"aes.h"
using namespace std;

void AES::print_key()
{
	for(const auto &a : W) {
		for(const auto& b : a) cout << hex << +b << ',';
		cout << endl;
	}
}

int main()
{
	AES aes;
	unsigned char key[16] = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 
							 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75},
				  key2[16] = {};
	aes.set_key(key);
	aes.print_key();
	aes.set_key(key2);
	aes.print_key();
}

