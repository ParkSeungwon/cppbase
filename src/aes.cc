#include<iostream>
#include"aes.h"
using namespace std;

void AES::key(const mpz_class key)
{
	mpz2bnd(key, key_, key_+ key_size_);
}

void AES::iv(const mpz_class iv)
{
	mpz2bnd(iv, iv_, iv_+16);
}

void AES2::key(int k)
{
	mpz_class k_ = k;
	AES::key(k_);
}

void AES2::iv(int i)
{
	mpz_class i_ = i;
	AES::iv(i_);
}

void AES2::show()
{
	cout << "key : 0x";
	for(int k : key_) cout << hex << k;
	cout << "iv : 0x";
	for(int k : iv_) cout << hex << k;
}
