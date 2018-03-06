#include <iostream>
#include <stdint.h>
using namespace std;

int main()
{
	uint32_t x = 13;
	cout << x << endl;
	short y = (short)x;
	short z = static_cast<short>(x);
	cout << y << endl;
	cout << z << endl;
	return 0;
}
