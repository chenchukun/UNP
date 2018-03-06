#include <pthread.h>
#include <iostream>
#include <sys/syscall.h> 
#include <unistd.h>
#include <functional>
using namespace std;

pid_t gettid() {return syscall(SYS_gettid);}

int main() 
{
	cout << gettid() << endl;
	function<void(void)> fun = NULL;
	return 0;
}
