#include "Block.h"
#include <iostream>
using namespace std;
using namespace snf;

int main()
{
	Block block1(12);
	block1.append("hello");
	cout << block1 << endl;
	block1.append(" world!!");
	cout << block1 << endl;
	block1.append(" woc!", 5);
	cout << block1 << endl;

	Block block2(string("hello world"));
	cout << block2 << endl;


	return 0;
}
