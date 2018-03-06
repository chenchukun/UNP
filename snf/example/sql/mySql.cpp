#include "MySql.h"
#include <iostream>
using namespace std;
using namespace snf;

int main()
{
	MySql mysql;
	int ret = mysql.connect("root", "123456");
	cout << ret << endl;
	return 0;
}
