#pragma once
#include<valarray>
#include<iostream>
#include<wolfssl/wolfcrypt/sha.h>
#include<wolfssl/wolfcrypt/sha256.h>
#define WOLFSSL_SHA512
#include<wolfssl/wolfcrypt/sha512.h>

class SHA1
{
public:
	static const int block_size = 64;
	static const int output_size = 20;
	SHA1() {
		if(wc_InitSha(&sha_)) std::cerr << "wc_init_sha_failed" << std::endl;
	}
	template<typename It>
	std::array<unsigned char, output_size> hash(const It begin, const It end) {
		std::array<unsigned char, output_size> r;
		wc_ShaUpdate(&sha_, &*begin, end - begin);
		wc_ShaFinal(&sha_, r.data());
		return r;
	}
	auto hash2(std::vector<unsigned char> v) {
		return hash(v.cbegin(), v.cend());
	}
protected:
	Sha sha_;
};

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
	auto hash2(std::vector<unsigned char> v) {
		return hash(v.cbegin(), v.cend());
	}
protected:
	Sha256 sha_;
};

class SHA5
{//sha512
public:
	static const int block_size = 128;
	static const int output_size = 64;
	SHA5() {
		if(wc_InitSha512(&sha_)) std::cerr << "wc_init_sha512_failed" << std::endl;
	}
	template<class It>
	std::array<unsigned char, output_size> hash(const It begin, const It end) {
		std::array<unsigned char, output_size> r;
		wc_Sha512Update(&sha_, &*begin, end - begin);
		wc_Sha512Final(&sha_, r.data());
		return r;
	}
	auto hash2(std::vector<unsigned char> v) {
		return hash(v.cbegin(), v.cend());
	}
protected:
	Sha512 sha_;
};

template<class H> class HMAC
{//hmac using sha1
public:
	HMAC() : o_key_pad_(H::block_size), i_key_pad_(H::block_size)
	{ }
	template<typename It> void key(const It begin, const It end)
	{//if less than block size(sha1 16? 64?) pad 0, more than block size hash -> 20
		int length = end - begin;//below (int)0x0 : compiler confuse with null ptr
		std::valarray<unsigned char> key((int)0x0, H::block_size),
			out_xor(0x5c, H::block_size), in_xor(0x36, H::block_size);
		if(length > H::block_size) {
			auto h = sha_.hash(begin, end);
			for(int i=0; i<H::output_size; i++) key[i] = h[i];
		} else if(int i = 0; length < H::block_size)
			for(auto it = begin; it != end; it++) key[i++] = *it;

		o_key_pad_ = key ^ out_xor;
		i_key_pad_ = key ^ in_xor;
	}
	template<typename It> auto hash(const It begin, const It end)
	{
		std::vector<unsigned char> v;
		v.insert(v.begin(), std::begin(i_key_pad_), std::end(i_key_pad_));
		v.insert(v.end(), begin, end);
		auto h = sha_.hash(v.begin(), v.end());
		v.clear();
		v.insert(v.begin(), std::begin(o_key_pad_), std::end(o_key_pad_));
		v.insert(v.end(), h.begin(), h.end());
		return sha_.hash(v.begin(), v.end());
	}
	void key2(std::vector<unsigned char> v) {
		key(v.cbegin(), v.cend());
	}
	auto hash2(std::vector<unsigned char> v) {
		return hash(v.cbegin(), v.cend());
	}
protected:
	H sha_;
	std::valarray<unsigned char> o_key_pad_, i_key_pad_;
};

template<class H> class PRF
{//H is hash function usually sha256
public:
	template<class It> void secret(const It begin, const It end) {
		secret_.clear();
		for(It it = begin; it != end; it++) secret_.push_back(*it);
		hmac_.key(secret_.begin(), secret_.end());
	}
	void secret2(std::vector<unsigned char> v) {
		secret(v.cbegin(), v.cend());
	}
	void label(const char* p) {
		label_.clear();
		while(*p) label_.push_back(*p++);
	}
	void label2(std::string s) {
		s += '\0';
		label(s.data());
	}
	template<class It> void seed(const It begin, const It end) {
		seed_.clear();
		for(It it = begin; it != end; it++) seed_.push_back(*it);
	}
	void seed2(std::vector<unsigned char> v) {
		seed(v.cbegin(), v.cend());
	}
	std::vector<unsigned char> get_n_byte(int n) {
		auto seed = label_;//seed = label + seed_
		seed.insert(seed.end(), seed_.begin(), seed_.end());
		std::vector<unsigned char> r, v;
		std::vector<std::array<unsigned char, H::output_size>> vA;
		vA.push_back(hmac_.hash(seed.begin(), seed.end()));//A(1)
		while(r.size() < n) {
			v.clear();
			v.insert(v.end(), vA.back().begin(), vA.back().end());
			v.insert(v.end(), seed.begin(), seed.end());
			auto h = hmac_.hash(v.begin(), v.end());
			r.insert(r.end(), h.begin(), h.end());
			vA.push_back(hmac_.hash(vA.back().begin(), vA.back().end()));//A(i+1)
		}
		while(r.size() != n) r.pop_back();
		return r;
	}

protected:
	HMAC<H> hmac_;
	std::vector<unsigned char> secret_, label_, seed_;
};


