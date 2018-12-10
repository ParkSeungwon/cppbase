#include<cstring>
#include<iostream>
#include<nettle/aes.h>
#include<nettle/cbc.h>
using namespace std;

int main()
{
	struct {
		aes128_ctx ctx_;
		uint8_t iv_[16];
	} ctx;
	uint8_t key[16] = {0x23, 0x1a, 0x65, 0x0e, 0xcf, 0xc5, 0x78, 0xbe,
					   0xa4, 0xb0, 0x46, 0x7a, 0xda, 0xab, 0x33, 0xc2};
	uint8_t iv[16] = {0x4c, 0x63, 0x01, 0x4c, 0x67, 0x1a, 0x8b, 0xae,
					  0x25, 0xb2, 0x2d, 0xef, 0x0f, 0x57, 0xce, 0x79};
	uint8_t dst[48];
	const uint8_t src[48] = {0x14, 0x00, 0x00, 0x0c, 0x48, 0x03, 0x7b, 0x71,
						   0x49, 0x97, 0x92, 0x96, 0x15, 0x15, 0xc4, 0x36,
						   0xf4, 0xc6, 0x33, 0x0f, 0x87, 0x48, 0x59, 0x36,
						   0x25, 0x43, 0x52, 0x72, 0x72, 0x4d, 0x39, 0x63,
						   0x2c, 0xdf, 0x4c, 0x06, 0x0b, 0x0b, 0x0b, 0x0b,
						   0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b};
	for(int i=0; i<3; i++) 
		cbc_encrypt(&ctx, (nettle_cipher_func*)aes128_encrypt, AES_BLOCK_SIZE, iv, 16,
				dst+16*i, src+16*i);
	for(uint8_t c : dst) cout << +c << ',';
	cout << endl << "block size" << AES_BLOCK_SIZE << endl;
}