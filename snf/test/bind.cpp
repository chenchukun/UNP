#include <functional>
#include <iostream>
using namespace std;

int fun(int x, int y)
{
	return 0;
}

class Test
{
public:
	void fun(int x) {
		cout << x << endl;
	}
};

int main()
{
	auto f = bind(fun, 1, 2);
	cout << sizeof(f) << endl;

	Test test;
	auto fun = bind(&Test::fun, &test, 1);
	fun();
	return 0;
}
