#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Data 
{
public:
	Data() {
		cout << "Data()" << endl;
	}

	~Data() {
		cout << "~Data()" << endl;
	}

	Data(const Data &data) {
		cout << "Data(const Data &data)" << endl;
	}

	Data(Data &&data) noexcept {
		cout << "Data(Data &&data)" << endl;
	}
};

int main()
{
	Data data;
	vector<Data> datas;
	datas.push_back(move(data));
	return 0;
}
