#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include<boost/python.hpp>
#include"aes.h"
using namespace boost::python;
using namespace std;

boost::python::list base64_dec(string s);
string base64_enc(boost::python::list l);
string pemtojson(string filename);
string dertojson(string filename);
void print(int n);
struct K
{
	void show() { cout << "K" << endl; }
	int k = 32;
};

BOOST_PYTHON_MODULE(tls_crypt) {
	def("base64_encode", &base64_enc);
	def("base64_decode", &base64_dec);
	def("pem2json", &pemtojson);
	def("der2json", &dertojson);
	def("print", &print);
	class_<K>("K")
		.def("show", &K::show)
		.def_readwrite("k", &K::k)
		;
	class_<AES2>("AES2")
		.def("key", &AES2::key)
		.def("iv", &AES2::iv)
		.def("show", &AES2::show)
		;
}

