#pragma once
#include<wolfssl/wolfcrypt/sha256.h>
#include<boost/python.hpp>

std::vector<unsigned char> l2v(boost::python::list l);

class SHA2
{//sha256, sha2 due to some naming reason
public:
	static const int block_size = 64;
	static const int output_size = 32;
	SHA2() {
		if(wc_InitSha256(&sha_)) std::cerr << "wc_init_sha256_failed" << std::endl;
	}
	template<typename It>
	std::array<unsigned char, output_size> hash(const It begin, const It end) {
		std::array<unsigned char, output_size> r;
		wc_Sha256Update(&sha_, (const byte*)&*begin, end - begin);
		wc_Sha256Final(&sha_, r.data());
		return r;
	}
	boost::python::list hash2(boost::python::list l) {
		auto v = l2v(l);
		auto a = hash(v.cbegin(), v.cend());
		boost::python::list r;
		for(unsigned char c : a) r.append(c);
		return r;
	}
protected:
	Sha256 sha_;
};

