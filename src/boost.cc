#include<boost/python/suite/indexing/vector_indexing_suite.hpp>
#include<boost/python.hpp>
#include"aes.h"
#include"sha.h"
using namespace boost::python;
using namespace std;

boost::python::list base64_dec(string s);
string base64_enc(boost::python::list l);
string pemtojson(string filename);
string dertojson(string filename);

BOOST_PYTHON_MODULE(tls_crypt) {
	def("base64_encode", &base64_enc);
	def("base64_decode", &base64_dec);
	def("pem2json", &pemtojson);
	def("der2json", &dertojson);
	class_<vector<unsigned char>>("vecz")
		.def(vector_indexing_suite<vector<unsigned char>>());
	class_<AES>("AES")
		.def("key", &AES::key2)
		.def("iv", &AES::iv2)
		.def("show", &AES::show)
		.def("encrypt", &AES::encrypt2)
		.def("decrypt", &AES::decrypt2)
		;
	class_<SHA2>("SHA256")
		.def("hash", &SHA2::hash2);
	class_<SHA1>("SHA1")
		.def("hash", &SHA1::hash2);
	class_<SHA5>("SHA512")
		.def("hash", &SHA5::hash2);
	class_<HMAC<SHA2>>("HMAC")
		.def("key", &HMAC<SHA2>::key2)
		.def("hash", &HMAC<SHA2>::hash2)
		;
	class_<PRF<SHA2>>("PRF")
		.def("label", &PRF<SHA2>::label2)
		.def("seed", &PRF<SHA2>::seed2)
		.def("secret", &PRF<SHA2>::secret2)
		.def("get_n_byte", &PRF<SHA2>::get_n_byte)
		;
}
