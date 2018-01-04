//
// Created by chenchukun on 18/1/1.
//

#include "SimpleBuffer.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

void test(SimpleBuffer &buffer) {
    if (buffer.readPosition() > buffer.writePosition()) {
        cerr << "ERROR: readPosition = " << buffer.readPosition() << "writePosition = " << buffer.writePosition() << endl;
        cerr << "ERROR: readPosition > writePosition" << endl;
        exit(-1);
    }
    if (buffer.writePosition() > buffer.capacity()) {
        cerr << "ERROR: readPosition = " << buffer.readPosition() << "writePosition = " << buffer.writePosition() << endl;
        cerr << "ERROR: writePosition > capacity" << endl;
        exit(-1);
    }
}

void testRead(SimpleBuffer &buffer) {
    int size = buffer.readSize();
    if (size > 0) {
        int n = rand() % size;
        buffer.setReadPosition(buffer.readPosition() + n);
        cout << "read " << n << " byte, readPosition = " << buffer.readPosition() << ", writePosition = " << buffer.writePosition() << endl;
        test(buffer);
    }
}

void testWrite(SimpleBuffer &buffer) {
    int size = buffer.writeSize();
    if (size > 0) {
        int n = rand() % size;
        buffer.setWritePosition(buffer.writePosition() + n);
        cout << "write " << n << " byte, readPosition = " << buffer.readPosition() << ", writePosition = " << buffer.writePosition() << endl;
        test(buffer);
    }
}

int main(int argc, char **argv)
{
    SimpleBuffer buffer;
    srand(time(NULL));
    for (int i=0; i<stoi(argv[1]); ++i) {
        int type = rand() % 9;
 //       cout << "type = " << type << endl;
        switch (type) {
            case 0:
            case 1:
            case 2:
                testRead(buffer);
                break;
            case 3:
            case 4:
            case 5:
                testWrite(buffer);
                break;
            case 6:
                buffer.clear();
                cout << "clear: readPosition = " << buffer.readPosition() << ", writePosition = " << buffer.writePosition() << endl;
                break;
            case 7:
                buffer.compact();
                cout << "compact: readPosition = " << buffer.readPosition() << ", writePosition = " << buffer.writePosition() << endl;
                break;
            case 8:
                size_t n = buffer.readSize() + rand() % 100;
                cout << "resize: capacity = " << buffer.capacity() << ", n = " << n << endl;
                buffer.resize(n);
                cout << "resize: readPosition = " << buffer.readPosition() << ", writePosition = " << buffer.writePosition() << ", capacity = " << buffer.capacity() << endl;
                break;
        }
    }
    return 0;
}
