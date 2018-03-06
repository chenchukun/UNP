#include <iostream>
#include <typeinfo>
using namespace std;

class Base
{
public:
	Base() {}
	~Base() {}

private:
	int x;
};

class Extend : public Base
{
public:
	Extend() {}
	~Extend() {}

private:
	int y;
};

int main()
{
	cout << (typeid(int) == typeid(10)) << endl;
	cout << typeid(int).name() << endl;
	cout << (typeid(Base) == typeid(Base)) << endl;
	Base *b = new Extend;
	cout << (typeid(b) == typeid(Base*)) << endl;
	cout << (typeid(*b) == typeid(Extend)) << endl;
	const type_info &info = typeid(*b);
	cout << info.name() << endl;
	return 0;
}
