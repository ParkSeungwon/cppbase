#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include<boost/python.hpp>
using namespace boost::python;
using namespace std;

const char *greet() {
	return "hello world";
}

vector<int> addv(vector<int> &v) {
	v.push_back(1);
	return v;
}
struct K {
	string greet() { return msg; }
	string msg;
	vector<int> v = {1, 2};
	void set(string m) { msg = m; }
	void addv(vector<int> w) {
		for(int k : w) v.push_back(k);
	}
	void push_back(int k) {
		v.push_back(k);
	}
};
BOOST_PYTHON_MODULE(hello) {
	def("greet", greet);
	def("vadd", addv);
//	class_<vector<int>>("vecint")
//		.def("push_back", &vector<int>::push_back)
//		;
	class_<vector<int>>("vecz")
		.def(vector_indexing_suite<vector<int>>())
		;
	class_<K>("K")
		.def("greet", &K::greet)
		.def("set", &K::set)
		.def("addv", &K::addv)
		.def("push_back", &K::push_back)
		.def_readwrite("msg", &K::msg)
		.def_readwrite("v", &K::v)
		;
}
