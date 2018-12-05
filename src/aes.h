#include<vector>
#include<gmpxx.h>
#include<cassert>
#include<wolfssl/wolfcrypt/aes.h>
#include<pybind11/pybind11.h>
#include"mpz.h"

class AES
{
public:
	AES(unsigned short bit = 128);
	void key2(pybind11::int_ key);
	void iv2(pybind11::int_ iv);
	void key(const mpz_class key);
	void iv(const mpz_class iv);
	std::vector<unsigned char> encrypt2(std::vector<unsigned char> msg);
	std::vector<unsigned char> decrypt2(std::vector<unsigned char> enc);
	std::string show();
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

class DiffieHellman
{//256 byte = 2048 bit
public:
	DiffieHellman(int bit_sz = 1024);
	DiffieHellman(mpz_class p, mpz_class g, mpz_class ya);
	DiffieHellman(pybind11::int_ p, pybind11::int_ g, pybind11::int_ ya);
	mpz_class set_yb(mpz_class pub_key);
	pybind11::int_ set_yb2(pybind11::int_ pub_key);
	mpz_class p = mpz_class{"0xd6c094ad57f5374f68d58c7b096872d945cee1f82664e0594421e1d5e3c8e98bc3f0a6af8f92f19e3fef9337b99b9c93a055d55a96e425734005a68ed47040fdf00a55936eba4b93f64cba1a004e4513611c9b217438a703a2060c2038d0cfaaffbba48fb9dac4b2450dc58cb0320a0317e2a31b44a02787c657fb0c0cbec11d"};
	mpz_class g = mpz_class{"0x27e1ab131b6c22d259d199e9df8acbb1fe2fd4461afb7cb321d6946b02c66a9a45c062d5ffd01e47075cf7b082845e87e49529a66a8405354d1148184933078341c9fa627fde3c2a9a195e2cae33145c47bd86bbcd49b012f235bbc58486ce1d75522175fc7c9efd3aeaac06855b003e65a2208d16e7d89d9359dfd5e7002de1"};
	mpz_class ya, yb, K;
	pybind11::int_ get_p();
	pybind11::int_ get_g();
	pybind11::int_ get_K();
	pybind11::int_ get_ya();
	pybind11::int_ get_yb();

protected:
	mpz_class xa, xb;
private:
	mpz_class premade = mpz_class{"0xb0a108069c0813ba59063cbc30d5f500c14f44a7d6ef4ac625271ce8d296530a5c91dda2c29484bf7db2449f9bd2c18ac5be725ca7e791e6d49f7307855b6648c770fab4ee02c93d9a4ada3dc1463e1969d1174607a34d9f2b9617396d308d2af394d375cfa075e6f2921f1a7005aa04835730fbda76933850e827fd63ee3ce5b7c809ae6f50358e84ce4a00e9127e5a31d733fc211376cc1630db0cfcc562a735b8efb7b0acc036f6d9c94648f94090002b1baa6ce31ac30b039e1bc246e4484e22736fc35fd49ad6300748d68c90abd4f6f1e348d3584ba6b9cd29bf681f084b63862f5c6bd6b60665f7a6dc00676bbbc3a94183fbc7fac8e21e7eaf003f93"};
};

class RSA
{
public:
	RSA(int key_size);//generate key
	RSA(mpz_class e, mpz_class d, mpz_class K);//read frem certificate or memory
	RSA(pybind11::int_ e, pybind11::int_ d, pybind11::int_ K);
	mpz_class sign(mpz_class m), decode(mpz_class m), encode(mpz_class m);
	pybind11::int_ encode(pybind11::int_ m), decode(pybind11::int_ m);
	mpz_class K, e;
protected:
	mpz_class p, q, d, phi;
};

