#include "Any.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
using namespace std;
using namespace snf;



int main()
{
	Any x = 12;
	int n;
	assert(x.castTo(n) == 0);
	cout << n << endl;
	Any s = string("string");
	string str;
	assert(s.castTo(str) == 0);
	cout << str << endl;

	return 0;
}
