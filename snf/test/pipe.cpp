#include <unistd.h>
#include <iostream>
#include <assert.h>
using namespace std;

int main()
{
	int fd[2];
	assert(pipe(fd)==0);
	cout << fpathconf(fd[0], _PC_PIPE_BUF) << endl;
	return 0;
}
