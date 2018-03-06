#include <iostream>
using namespace std;

class Base
{
	virtual void method() {cout << "0";}
public:
	virtual ~Base() {method();}

	void baseMethod() {method();}
};

class A : public Base 
{
	void method() {cout << "1";}
public:
	~A() {method();}
};

class Base1
{
public:
	Base1() {}

	virtual ~Base1() {}

	virtual void print() {}

	virtual Base1 *clone() const {}

protected:
	int data_Base1;
};

class Base2
{
public:
	Base2() {}

	virtual ~Base2() {}

	virtual Base2 *clone() const {}

	virtual void print() {}

protected:
	float data_Base2;
};

class Derived : public Base1, public Base2
{
public:
	Derived() {}

	virtual ~Derived() {}

	virtual Derived *clone() const {}

protected:
	static double data_Derived;
};

class Test
{
	virtual void fun() {}
private:
//	int x;
};

int main()
{
	Base *base = new A;
	base->baseMethod(); // 1
	delete base;		// 1 0
	cout << endl;
	cout << sizeof((void*)NULL) << endl;	// 8
	cout << sizeof(int) << endl;
	cout << sizeof(float) << endl;
	cout << sizeof(Base1) << endl;  // 12
	cout << sizeof(Base2) << endl;	// 12
	cout << sizeof(Derived) << endl;	// 16
	cout << sizeof(Test) << endl;
	return 0;
}
