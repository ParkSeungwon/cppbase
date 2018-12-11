#include<iostream>
#include"aes.h"
using namespace std;

void AES::set_key(unsigned char *key) {
	memcpy(W[0], key, 16);
	unsigned char *p = &W[1][0];
	for(int i=1; i<R; i++) {
		for(int j=0; j<3; j++) *(p+j) = *(p+j-3);
		*(p+3) = *(p-4);
		for(int j=0; j<4; j++) *(p+j) = sbox[*(p+j) / 0x100][*(p+j) % 0x100];
		for(int j=0; j<4; j++, p++) {//p+=4
			*p ^= rcon[4*i/N-1][j];
			*p ^= *(p - 4*N);
		}
		for(int j=0; j<12; j++, p++) *p = *(p - 4*N) ^ *(p - 4);//p+=12
	}
}

void AES::print_key()
{
	for(const auto& a : W) {
		for(const auto& b : a) cout << hex << + b << ',';
		cout << endl;
	}
}

void AES::set_iv(unsigned char *p)
{
	memcpy(iv_, p, 16);
}

void AES::encrypt(unsigned char *m)
{
	for(int i=0; i<16; i++)  m[i] ^= W[0][i];
	for(int round=1; round<11; round++) {
		substitute(m);
		shift_row(m);
		if(round != 10) mix_column(m);
		add_round_key(m, round);
	}
}

void AES::decrypt(unsigned char *p)
{
	add_round_key(p, R-1);
	for(int round=R-2; round>=0; round--) {
		inv_shift_row(p);
		inv_substitute(p);
		add_round_key(p, round);
		if(round) inv_mix_column(p);
	}
}

void AES::substitute(unsigned char *p)
{
	for(int i=0; i<16; i++) p[i] = sbox[p[i] / 0x100][p[i] % 0x100];
}

void AES::inv_substitute(unsigned char *p)
{
	for(int i=0; i<16; i++) p[i] = inv_sbox[p[i] / 0x100][p[i] % 0x100];
}

void AES::shift_row(unsigned char *p)
{
	unsigned char tmp, tmp2;
	tmp = p[1]; p[1] = p[5]; p[5] = p[9]; p[9] = p[13]; p[13] = tmp;
	tmp = p[2]; tmp2 = p[6]; p[2] = p[10]; p[6] = p[14]; p[10] = tmp; p[14] = tmp2;
	tmp = p[3]; p[3] = p[15]; p[15] = p[11]; p[11] = p[7]; p[7] = tmp;
}

void AES::inv_shift_row(unsigned char *p)
{
	unsigned char tmp, tmp2;
	tmp = p[13]; p[13] = p[9]; p[9] = p[5]; p[5] = p[1]; p[1] = tmp;
	tmp = p[10]; tmp2 = p[14]; p[14] = p[6]; p[10] = p[2]; p[6] = tmp2; p[2] = tmp;
	tmp = p[7]; p[7] = p[11]; p[11] = p[15]; p[15] = p[3]; p[3] = tmp;
}

void AES::mix_column(unsigned char *p)
{
	static const unsigned char mix[4][4] 
		= {{2,3,1,1}, {1,2,3,1}, {1,1,2,3}, {3,1,1,2}};
	unsigned char c[4], d, result[16];
	for(int y=0; y<4; y++) for(int x=0; x<4; x++) {
		for(int i=0; i<4; i++) {
			d = p[4*x + i];
			switch(mix[y][i]) {
				case 1: c[i] = d; 			break;
				case 2: c[i] = d << 1;	 	break;
				case 3: c[i] = d << 1 ^ d;	break;
			}
			if((d & 1<<7) && (mix[y][i] != 1)) c[i] ^= 0x1b;//결합법칙덕분
		}
		result[4*x + y] = c[0] ^ c[1] ^ c[2] ^ c[3];
	}
	memcpy(p, result, 16);
}

void AES::inv_mix_column(unsigned char *p)
{
	static const unsigned char mix[4][4] = {
		{14, 11, 13, 9}, {9, 14, 11, 13}, {13, 9, 14, 11}, {11, 13, 9, 14}};
	unsigned char c[4], d, result[16];
	for(int y=0; y<4; y++) for(int x=0; x<4; x++) {
		for(int i=0; i<4; i++) {
			d = p[4*x + i];
			switch(mix[y][i]) {
				case 9: c[i] = doub(doub(doub(d))) ^ d;		break;
				case 11: c[i] = doub(doub(doub(d)) ^ d) ^ d;break;
				case 13: c[i] = doub(doub(doub(d) ^ d)) ^ d;break;
				case 14: c[i] = doub(doub(doub(d) ^ d) ^ d);break;
			}
		}
		result[4*x + y] = c[0] ^ c[1] ^ c[2] ^ c[3];
	}
	memcpy(p, result, 16);
}

unsigned char AES::doub(unsigned char c)
{
	bool left_most_bit = c & 1 << 7;
	c <<= 1;
	if(left_most_bit) c ^= 0x1b;
	return c;
}

void AES::add_round_key(unsigned char *p, int k)
{
	for(int i=0; i<16; i++) p[i] ^= W[k][i];
}
