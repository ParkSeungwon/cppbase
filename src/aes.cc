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

Mpz::Mpz(pb::int_ i) : mpz_class{pb::str(i)}
{ }

Mpz::Mpz(const mpz_class &i) : mpz_class{i}
{ }

Mpz::operator pybind11::int_()
{
	return pb::str(get_str());
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

DiffieHellman::DiffieHellman(Mpz p, Mpz g, Mpz ya)
{//client side
	this->p = p; this->g = g; this->ya = ya;
	if(mpz_sizeinbase(p.get_mpz_t(), 16) <= 256) xb = random_prime(128);
	else xb = random_prime(256);
	yb = powm(g, xb, p);
	K = powm(ya, xb, p);
}

pb::int_ DiffieHellman::get_p() { return pb::str(p.get_str()); }
pb::int_ DiffieHellman::get_g() { return pb::str(g.get_str()); }
pb::int_ DiffieHellman::get_K() { return pb::str(K.get_str()); }
pb::int_ DiffieHellman::get_ya() { return pb::str(ya.get_str()); }
pb::int_ DiffieHellman::get_yb() { return pb::str(yb.get_str()); }

pb::int_ DiffieHellman::set_yb(pb::int_ pub_key)
{//set client pub key
	yb = Mpz{pub_key};
	K = powm(yb, xa, p);
	return Mpz{K};
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

RSA::RSA(Mpz e, Mpz d, Mpz K)
{
	this->e = e;
	this->d = d;
	this->K = K;
}	

pb::int_ RSA::encode(pb::int_ m)
{//K should be bigger than m
	return Mpz{powm(Mpz{m}, e, K)};
}
pb::int_ RSA::decode(pb::int_ m)
{
	return Mpz{powm(Mpz{m}, d, K)};
}

