#include<cstring>
#include"sha1.h"
using namespace std;

void SHA1::preprocess(vector<unsigned char> &v)
{
	auto it = v.end();
	v.insert(it, v.begin(), v.end());
	v.insert(it, 0x80);
	size_t n = 64 - v.size() % 64;
	v.insert(it + 1, n == 64 ? 0 : n, 0);
}

void SHA1::process_chunk(unsigned char *p)
{
	uint32_t w[80];
	memcpy(w, p, 64);
	for(int i=16; i<80; i++) 
		w[i] = left_rotate(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
	uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4] , f, tmp;
	uint8_t k[4] = { 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 };
	uint32_t pk;
	for(int i=0; i<80; i++) {
		switch(i/20) {
			case 0: f = (b & c) | (~b & d);				pk = k[0]; break;
			case 1: f = b ^ c ^ d;						pk = k[1]; break;
			case 2: f = (b & c) | (b & d) | (c & d);	pk = k[2]; break;
			case 3: f = b ^ c ^ d;			 			pk = k[3]; break;
		}
		tmp = left_rotate(a, 5) + f + e + pk + w[i];
		e = d; d = c; c = left_rotate(b, 30); b = a; a = tmp;
	}
	h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
}

uint32_t SHA1::left_rotate(uint32_t k, int bits)
{//work in big endian as well as little endian
	uint32_t front = k << bits;
	uint32_t back = k >> (32 - bits);
	return front | back;
}

void SHA1::bndplus(unsigned char *a, unsigned char *b)
{//a += b
	
}
