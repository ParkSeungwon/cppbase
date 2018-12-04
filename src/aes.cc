#include<deque>
#include"aes.h"
using namespace std;
namespace bp = boost::python;

bp::long_ mpz2long(mpz_class l)
{
	bp::long_ r; 
	deque<unsigned> q;
	const unsigned i = 1 << 31;
	for(; l; l /= i) q.push_front(mpz_class{l % i}.get_ui());
	for(unsigned k : q) {
		r *= i;
		r += k;
	}
	return r;
}

mpz_class long2mpz(bp::long_ l) 
{
	mpz_class r = 0;
	deque<unsigned> q;
	const unsigned i = 1 << 31;
	for(; l; l /= i) q.push_front(bp::extract<unsigned>(l % i));
	for(unsigned k : q) {
		r *= i;
		r += k;
	}
	return r;
}

AES::AES(unsigned short key_size) : key_size_{key_size / 8}
{ }

void AES::key(const mpz_class key)
{
	mpz2bnd(key, key_, key_+ key_size_);
}

void AES::key2(bp::long_ k)
{
	key(long2mpz(k));
}

void AES::iv(const mpz_class iv)
{
	mpz2bnd(iv, iv_, iv_+16);
}

void AES::iv2(bp::long_ i)
{
	iv(long2mpz(i));
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

vector<unsigned char> l2v(bp::list l);
bp::list v2l(vector<unsigned char> v);
bp::list AES::encrypt2(bp::list msg)
{
	auto v = l2v(msg);
	return v2l(encrypt(v.begin(), v.end()));
}

bp::list AES::decrypt2(bp::list enc)
{
	auto v = l2v(enc);
	return v2l(decrypt(v.begin(), v.end()));
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
mpz_class DiffieHellman::set_yb(mpz_class pub_key)
{//set client pub key
	yb = pub_key;
	K = powm(yb, xa, p);
	return K;
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

