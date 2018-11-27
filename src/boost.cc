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
	class_<AES>("AES")
		.def("key", &AES::key2)
		.def("iv", &AES::iv2)
		.def("show", &AES::show)
		.def("encrypt", &AES::encrypt2)
		.def("decrypt", &AES::decrypt2)
		;
	class_<SHA2>("SHA256")
		.def("hash", &SHA2::hash2)
		;
}
