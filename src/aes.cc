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
