#include"clmat.h"
using namespace std;

class Test : public CLMat
{
	Test() : CLMat{{{3,4,5},{2,3,1}}} {
		repeat(3);
	}


};

int main()
{
	try{
	CLMat m{{ {3.1,4,5},{2,3,1} }};
	m.show();
	CLMat n{{ {3, 1},{2, 2},{1, 5} }};
	n.show();
	auto m2 = m * n;
	m.show();n.show();
	cout << m << n << m2 << endl;
	cout << m << endl;
	} catch(const char* e) {cout << e << endl;}
}

