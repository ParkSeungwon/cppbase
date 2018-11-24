#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include<boost/python.hpp>
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
}
