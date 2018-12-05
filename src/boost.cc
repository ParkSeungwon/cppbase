#include<pybind11/pybind11.h>
#include<pybind11/stl.h>
#include<pybind11/iostream.h>
#include<functional>
#include"aes.h"
#include"sha.h"
using namespace std;
using namespace pybind11;
using namespace pybind11::literals;

string base64_encode(vector<unsigned char> v);
vector<unsigned char> base64_decode(string s);
string pemtojson(string filename);
string dertojson(string filename);

PYBIND11_MODULE(tls_crypt, m) {
	m.def("base64_encode", &base64_encode);
	m.def("base64_decode", &base64_decode);
	m.def("pem2json", &pemtojson);
	m.def("der2json", &dertojson);
	class_<AES>(m, "AES")
		.def(init<unsigned short>(), "bit"_a = 128)
		.def("key", &AES::key2)
		.def("iv", &AES::iv2)
		.def("show", &AES::show)
		.def("encrypt", &AES::encrypt2)
		.def("decrypt", &AES::decrypt2)
		;
	class_<SHA2>(m, "SHA256")
		.def(init<>())
		.def("hash", &SHA2::hash2);
	class_<SHA1>(m, "SHA1")
		.def(init<>())
		.def("hash", &SHA1::hash2);
	class_<SHA5>(m, "SHA512")
		.def(init<>())
		.def("hash", &SHA5::hash2);
	class_<HMAC<SHA2>>(m, "HMAC")
		.def(init<>())
		.def("key", &HMAC<SHA2>::key2)
		.def("hash", &HMAC<SHA2>::hash2)
		;
	class_<PRF<SHA2>>(m, "PRF")
		.def(init<>())
		.def("label", &PRF<SHA2>::label2)
		.def("seed", &PRF<SHA2>::seed2)
		.def("secret", &PRF<SHA2>::secret2)
		.def("get_n_byte", &PRF<SHA2>::get_n_byte)
		;
	class_<DiffieHellman>(m, "DiffieHellman")
		.def(init<int>(), "bit"_a = 1024)
		//.def(init<mpz_class, mpz_class, mpz_class>())
		.def(init<int_, int_, int_>())
		.def("set_yb", &DiffieHellman::set_yb2)
		.def_property_readonly("K", &DiffieHellman::get_K)
		.def_property_readonly("p", &DiffieHellman::get_p)
		.def_property_readonly("g", &DiffieHellman::get_g)
		.def_property_readonly("ya", &DiffieHellman::get_ya)
		.def_property_readonly("yb", &DiffieHellman::get_yb)
		;
	class_<RSA>(m, "RSA")
		.def(init<int>(), "key_size"_a = 1024)
		.def(init<int_, int_, int_>())
		.def("encode", &RSA::encode)
		.def("decode", &RSA::decode)
		.def("sign", &RSA::decode)
		;
}
