#include <unistd.h>
#include <cstdio>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	int ch;
	// opterr为非0，则当解析错误时打印错误信息，为0则不打印错误信息，返回'?'或':'
	opterr = 0;
	int n = 0;
	// b:: 为可选参数，可选参数的值和参数之间不能有空格，如: -b192.168.1.1
	// 解析到一个不在optstring里的参数或一个必选参数不带值是，返回'?'
	// 当optstring以':'开头时，缺省参数的情况下会返回':'，而不是'?'
	while ( (ch=getopt(argc, argv, "a:b::cde"))!=-1 ) {
		// 当getopt检测到一个未知选项字符或者某个选项没有带参数的时候，
		// optopt将会存储该参数的值。
		if (ch == '?') {
			cout << char(optopt) << " option not exists." << endl;
			continue;
		}
		switch (ch) {
			case 'a':
				// optarg 表示选项的参数
				printf("option a:%s\n", optarg);
				n++;
				break;
			case 'b':
				printf("option b:%s\n", optarg);
				break;
			case 'c':
				printf("option c\n");
				break;
			default:
				printf("other option: %c\n", ch);
		}
	}
	// optind 表示下一个将被处理的argv数组中的元素，当getopt完成所有选项的解析时，
	// optind表示剩下的非选项参数的起点位置
	int index = optind;
	while (index < argc) {
		cout << argv[index++] << endl;
	}
	return 0;
}
