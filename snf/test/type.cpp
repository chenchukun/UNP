#include <iostream>
using namespace std;

template<typename TO, typename FROM>
TO implicit_cast(const FROM &f)
{
	return f;
}

int main() 
{
	float f = implicit_cast<float>(12);
	cout << f << endl;
	cout << sizeof(size_t) << endl;

	return 0;
}
