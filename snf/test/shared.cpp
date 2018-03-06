#include <memory>
#include <iostream>
using namespace std;

class Test
{
public:
	weak_ptr<Test> tid() {
		_tid.reset(this, [](Test *x){});
		weak_ptr<Test> wp = _tid;
		return wp;
	}

private:
	shared_ptr<Test> _tid;
};

weak_ptr<Test> fun()
{
	Test test;
	auto wp = test.tid();
	if (wp.lock() == NULL) {
		cout << "NULL" << endl;
	}
	else {
		cout << "NOT NULL" << endl;
	}
}

int main()
{
	auto wp = fun();
	if (wp.lock() == NULL) {
		cout << "NULL" << endl;
	}
	else {
		cout << "NOT NULL" << endl;
	}
	return 0;
}
