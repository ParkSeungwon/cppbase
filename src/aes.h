#include<vector>
#include<gmpxx.h>
#include<cassert>
#include<wolfssl/wolfcrypt/aes.h>
#include"mpz.h"

class AES
{
public:
	AES(unsigned short bit = 128);
	void key(const mpz_class key);
	void key(const unsigned char* key);
	void iv(const mpz_class iv);
	void iv(const unsigned char* iv);
	template<typename It>
	std::vector<unsigned char> encrypt(const It begin, const It end) {
		int sz = end - begin;
		assert(sz % 16 == 0);
		std::vector<unsigned char> result(sz);
		wc_AesSetKey(&aes_, key_, key_size_, iv_, AES_ENCRYPTION);
		wc_AesCbcEncrypt(&aes_, result.data(), (const byte*)&*begin, sz);//&* for iterator
		return result;
	}
	template<typename It>
	std::vector<unsigned char> decrypt(const It begin, const It end) {
		int sz = end - begin;
		assert(sz % 16 == 0);
		std::vector<unsigned char> result(sz);
		wc_AesSetKey(&aes_, key_, key_size_, iv_, AES_DECRYPTION);
		wc_AesCbcDecrypt(&aes_, result.data(), (const byte*)&*begin, sz);
		return result;
	}
protected:
	unsigned char key_[32], iv_[16];
	Aes aes_;
	unsigned char key_size_;
};

struct AES2 : public AES
{
	void key(int k);
	void iv(int k);
	void show();
};
