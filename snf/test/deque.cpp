#include <iostream>
#include <deque>
using namespace std;

int main()
{
	deque<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(3);
	for (int i=0; i<x.size(); ++i) {
		cout << x[i] << endl;
	}
	return 0;
}
