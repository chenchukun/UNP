#include <iostream>
#include <netinet/in.h>
using namespace std;

const char* serial()
{
	int x = 1;
//	x = htonl(x);
	const char *low = reinterpret_cast<const char*>(&x);
	if (*low == 1) {
		return "小端字节序";
	}
	return "大端字节序";
}

int main() 
{
	cout << serial() << endl;
	return 0;
}
