#pragma once
#include<gmpxx.h>
#include<iomanip>
#include<sstream>

template<typename It> void mpz2bnd(mpz_class n, It begin, It end)
{//mpz to big endian
	for(It i=end; i!=begin; n /= 0x100) *--i = mpz_class{n % 0x100}.get_ui();
}
template<typename It> mpz_class bnd2mpz(It begin, It end)
{//big endian to mpz
	std::stringstream ss; ss << "0x";
	for(It i=begin; i!=end; i++)
		ss << std::hex << std::setfill('0') << std::setw(2) << +*i;
	return mpz_class{ss.str()};
}

mpz_class nextprime(mpz_class n);
mpz_class random_prime(unsigned byte);
mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod);
