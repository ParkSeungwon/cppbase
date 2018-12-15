#include<cstring>
#include"sha1.h"
#include"log.h"
using namespace std;

void SHA1::preprocess(vector<unsigned char> &v)
{
	size_t sz = v.size();
	size_t n = (64 - (sz + 1) % 64) % 64;
	v.push_back(0x80);
	v.insert(v.end(), n, 0);
	for(auto it = v.rbegin(); sz; it++, sz /= 0x100) *it = sz % 0x100;
	LOGD << hexprint("preprocessed", v);
}

void SHA1::process_chunk(unsigned char *p)
{
	uint32_t w[80];
	memcpy(w, p, 64);
	for(int i=0; i<16; i++) w[i] = ntohl(w[i]);
	LOGD <<hex<< w[13] <<' '<<w[8] << ' ' << w[2] << ' ' << htonl(w[0]) << endl;
	LOGD << hex << (w[16-3] ^ w[16-8] ^ w[16-14] ^ w[16-16]) << endl;
	for(int i=16; i<80; i++)
		w[i] = left_rotate(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
	for(int i=0; i<80; i++) cout << hex << w[i] << ',';
	uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4] , f, tmp, pk;
	uint32_t k[4] = { 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 };
	for(int i=0; i<80; i++) {
		switch(i/20) {
			case 0: f = d ^ (b & (c ^ d));				break;
			case 1: f = b ^ c ^ d;						break;
			case 2: f = (b & c) | (b & d) | (c & d);	break;
			case 3: f = b ^ c ^ d;			 			break;
		}
//		LOGD << hex << a << ' ' << b << ' ' << c << ' ' << d << ' ' << e 
//			<< ' ' << f << ' ' << htonl(w[i]) << endl;
		tmp = left_rotate(a, 5) + f + e + k[i/20] + w[i];//!!!
//		LOGE << hex << left_rotate(a, 5) << endl;
		e = d; d = c; c = left_rotate(b, 30); b = a; a = tmp;
//		LOGD << hex << a << ' ' << b << ' ' << c << ' ' << d << ' ' << e << endl;
	}
	h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
}

uint32_t SHA1::left_rotate(uint32_t a, int bits)
{//works little endian as well as big endian
//	a = ntohl(a);
	return a << bits | a >> (32 - bits);
}
