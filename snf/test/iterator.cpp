#include <iostream>
#include <map>
using namespace std;

class Iterator
{
public:
	Iterator& operator++() {
		cout << "++" << endl;
		return *this;
	}
};

int main()
{
	Iterator *it = new Iterator;
	++it;
	map<int, int> m;
	auto mit = m.begin();

	return 0;
}
