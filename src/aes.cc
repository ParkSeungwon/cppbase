#include<iostream>
#include"aes.h"
using namespace std;

AES::AES(unsigned short key_size) : key_size_{key_size / 8}
{ }

void AES::key(const mpz_class key)
{
	mpz2bnd(key, key_, key_+ key_size_);
}

void AES::key2(string k)
{
	key(mpz_class{k});
}

void AES::iv(const mpz_class iv)
{
	mpz2bnd(iv, iv_, iv_+16);
}

void AES::iv2(string i)
{
	iv(mpz_class{i});
}
string AES::show()
{
	stringstream ss;
	ss << "key : 0x";
	for(int k : key_) ss << hex << k;
	ss << "\niv : 0x";
	for(int k : iv_) ss << hex << k;
	return ss.str();
}

vector<unsigned char> l2v(boost::python::list l);
boost::python::list v2l(vector<unsigned char> v);
boost::python::list AES::encrypt2(boost::python::list msg)
{
	auto v = l2v(msg);
	return v2l(encrypt(v.begin(), v.end()));
}

boost::python::list AES::decrypt2(boost::python::list enc)
{
	auto v = l2v(enc);
	return v2l(decrypt(v.begin(), v.end()));
}
