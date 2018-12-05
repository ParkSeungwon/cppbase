#include<iostream>
#include<deque>
#include<pybind11/stl.h>
#include"aes.h"
using namespace std;
namespace pb = pybind11;

AES::AES(unsigned short key_size) : key_size_{key_size / 8}
{ }

void AES::key(const mpz_class key)
{
	mpz2bnd(key, key_, key_+ key_size_);
}

void AES::key2(pb::int_ k)
{
	key(mpz_class{pb::str(k)});
}

void AES::iv(const mpz_class iv)
{
	mpz2bnd(iv, iv_, iv_+16);
}

void AES::iv2(pb::int_ i)
{
	iv(mpz_class{pb::str(i)});
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

vector<unsigned char> AES::encrypt2(vector<unsigned char> msg)
{
	return encrypt(msg.begin(), msg.end());
}

vector<unsigned char> AES::decrypt2(vector<unsigned char> enc)
{
	return decrypt(enc.begin(), enc.end());
}

DiffieHellman::DiffieHellman(int bit_sz)
{//server side
	if(bit_sz == 2048) {
		p = premade;
		g = 2;
	}
	xa = random_prime(bit_sz / 8);
	ya = powm(g, xa, p);
}
DiffieHellman::DiffieHellman(mpz_class p, mpz_class g, mpz_class ya)
{//client side
	this->p = p; this->g = g; this->ya = ya;
	if(mpz_sizeinbase(p.get_mpz_t(), 16) <= 256) xb = random_prime(128);
	else xb = random_prime(256);
	yb = powm(g, xb, p);
	K = powm(ya, xb, p);
}
DiffieHellman::DiffieHellman(pb::int_ p, pb::int_ g, pb::int_ ya) :
	DiffieHellman(mpz_class{pb::str(p)}, mpz_class{pb::str(g)}, mpz_class{pb::str(ya)})
{ }

pb::int_ DiffieHellman::get_p() { return pb::str(p.get_str()); }
pb::int_ DiffieHellman::get_g() { return pb::str(g.get_str()); }
pb::int_ DiffieHellman::get_K() { return pb::str(K.get_str()); }
pb::int_ DiffieHellman::get_ya() { return pb::str(ya.get_str()); }
pb::int_ DiffieHellman::get_yb() { return pb::str(yb.get_str()); }

mpz_class DiffieHellman::set_yb(mpz_class pub_key)
{//set client pub key
	yb = pub_key;
	K = powm(yb, xa, p);
	return K;
}

pb::int_ DiffieHellman::set_yb2(pb::int_ pub_key)
{
	set_yb(mpz_class{pb::str(pub_key)});
	return get_yb();
}

RSA::RSA(int key_size)
{
	p = random_prime(key_size / 2);
	q = random_prime(key_size / 2);
	K = p * q;
	phi = lcm(p-1, q-1);//e와 phi는 서로소
	for(e = 0x10001; gcd(e, phi) != 1; e = nextprime(e));
	mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());//modular inverse
}

RSA::RSA(mpz_class e, mpz_class d, mpz_class K)
{
	this->e = e;
	this->d = d;
	this->K = K;
}	

mpz_class RSA::encode(mpz_class m)
{//K should be bigger than m
	return powm(m, e, K);
}
mpz_class RSA::decode(mpz_class m)
{
	return powm(m, d, K);
}
mpz_class RSA::sign(mpz_class m)
{
	return decode(m);
}

