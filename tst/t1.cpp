#include"clmat.h"
using namespace std;

int main()
{
	CLMat m{{ {3,4,5},{2,3,1} }};
	CLMat n{{ {3},{2},{1} }};
	auto m2 = m * n;
	cout << m << n << m2 << endl;
	cout << m << endl;
}

