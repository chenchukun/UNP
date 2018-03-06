#include "IoHandle.h"
#include <cstring>
using namespace snf;

int main()
{
	IoHandle handle(STDOUT_FILENO);
	const char *buff = "Hello World!\n";
	write(handle.getHandle(), buff, strlen(buff));

	IoHandle handle2 = handle.dup();
	write(handle2.getHandle(), buff, strlen(buff));
	return 0;
}
