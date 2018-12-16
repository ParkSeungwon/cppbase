#include<cstring>
#include"sha1.h"
using namespace std;

static void byteReverse( uint32_t *buffer, int byteCount )
{
	for(uint32_t count = 0, value; count < byteCount; count++ ) {
		value = ( buffer[ count ] << 16 ) | ( buffer[ count ] >> 16 );
		buffer[count] = ((value & 0xFF00FF00L) >> 8) | ((value & 0x00FF00FFL) << 8);
	}
}

static uint32_t left_rotate(uint32_t a, int bits)
{
	return a << bits | a >> (32 - bits);
}

SHA1::SHA1()
{
	uint8_t a[4] = {0x12, 0x34, 0x56, 0x78};
	uint32_t *p = (uint32_t*)a;
	if(htonl(*p) == *p) big_endian_ = true;
}

void SHA1::preprocess(vector<unsigned char> &v)
{
	size_t sz = v.size();
	size_t n = (64 - (sz + 1) % 64) % 64;
	v.push_back(0x80);
	v.insert(v.end(), n, 0);
	for(auto it = v.rbegin(); sz; it++, sz /= 0x100) *it = sz % 0x100;
}

void SHA1::process_chunk(unsigned char *p)
{
	uint32_t w[80];
	memcpy(w, p, 64);
	//if(!big_endian_) for(int i=0; i<16; i++) w[i] = ntohl(w[i]);
	if(!big_endian_) byteReverse(w, 16);
	for(int i=16; i<80; i++)//bit operations are abstracted as numbers
		w[i] = left_rotate(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
	uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4] , f, tmp, pk;
	uint32_t k[4] = { 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 };
	for(int i=0; i<80; i++) {
		switch(i/20) {
			case 0: f = d ^ (b & (c ^ d));				break;
			case 1: f = b ^ c ^ d;						break;
			case 2: f = (b & c) | (b & d) | (c & d);	break;
			case 3: f = b ^ c ^ d;			 			break;
		}
		tmp = left_rotate(a, 5) + f + e + k[i/20] + w[i];//!!!
		e = d; d = c; c = left_rotate(b, 30); b = a; a = tmp;
	}
	h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
}


