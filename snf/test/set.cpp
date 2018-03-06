#include <iostream>
using namespace std;

/*
void setPri(bool op) {
 71         int val = op? SNF_POLLPRI: 0;
 72         event |= val; 
 73     } 
 */

#define SET(name, value) void name(bool op) {event = op?event|value:event&~value;}

class Test
{
public:
	Test(): event(0) {}

	SET(setTwo, 2);

	int get() {
		return event;
	}

	Test test() {
		return *this;
	}

private:
	int event;
};

int main()
{
	Test test;
	cout << test.get() << endl;
	test.setTwo(true);
	cout << test.get() << endl;

	return 0;
}
