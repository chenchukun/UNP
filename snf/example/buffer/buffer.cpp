#include "Buffer.h"
#include <iostream>
using namespace std;
using namespace snf;

int main()
{
	Buffer buffer;
	cout << "-------write----------" << endl;
	buffer.append(string("hello world"));
	cout << buffer << endl;
	buffer.append(string("woc"));
	cout << buffer << endl;
	buffer.append(string(", jjy"));
	cout << buffer << endl;
	buffer.append(string("  kikuchanj.."));
	cout << buffer << endl;
	buffer.appendInt64(360);
	cout << buffer << endl;
	buffer.appendInt8(97);
	cout << buffer << endl;

	cout << "-------read----------" << endl;
	char buff[128];
	buffer.read(buff, 11);
	buff[11] = 0;
	cout << buff << endl;
	cout << buffer << endl;

	buffer.read(buff, 5);
	buff[5] = 0;
	cout << buff << endl;
	cout << buffer << endl;

	buffer.read(buff, 16);
	buff[16] = 0;
	cout << buff << endl;
	cout << buffer << endl;

	int64_t x = buffer.readInt64();
	cout << x << endl;
	cout << buffer << endl;

	int8_t y = buffer.readInt8();
	cout << y << endl;
	cout << buffer << endl;

	buffer.append(string("jjy, for kiku, kikuchanj."));
	cout << buffer << endl;
	
	buffer.read(buff, 3);
	buff[3] = 0;
	cout << buff << endl;
	cout << buffer << endl;

	buffer.append(string("jjy, for kiku\r\n, kikuchanj."));
	cout << buffer << endl;

	int index = buffer.findCRLF();
	cout << "CRLF index: " << index  << endl;
	string all;
	buffer.readAll(all);
	cout << "all: " << all << endl;
	cout << buffer << endl;

	buffer.append("hello world. hello hi \nwocao, \rjjy\r\nkikuchanj.");
	cout << buffer << endl;

	index = buffer.findCRLF();
	string cr;
	buffer.read(cr, index);
	cout << "cr:" << cr << endl;
	cout << buffer << endl;

	buffer.readFd(0);
	cout << buffer << endl;
	buffer.readFd(0);
	cout << buffer << endl;

	buffer.writeFd(1);
	return 0;
}
