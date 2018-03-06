#include <getopt.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
	int opt;
	int digitOptind = 0;
	int optionIndex = 0;
	opterr = 0;
	const char *optstring = "a:b:c:d";
	string optt = "hello";
	static struct option longOptions[] = {
		{"reqarg", required_argument, NULL, 'r'},
		{"noarg", no_argument, NULL, 'n'},
		{"optarg", optional_argument, NULL, 'o'},
		{optt.c_str(), no_argument, NULL, 'h'},
		{0, 0, 0, 0}
	};
	vector<struct option> options;
	options.push_back(option{0,0,0,0});
	options.insert(options.begin(), option{"reqarg", required_argument, NULL, 'r'});
	options.insert(options.begin(), option{"noarg", no_argument, NULL, 'n'});
	cout << options.size() << endl;
	for(int i=0; i<options.size(); ++i) {
		std::cout << options[i].name << "  "
				  << options[i].has_arg << "  "
				  << options[i].flag << "  "
				  << options[i].val << std::endl;
	} 


	// getopt_long 和 getopt_long_only类型，只是getopt_long_only可以匹配-name为长参数
	// 而getopt_long 会将-name匹配为-n -a -m -e
	while ( (opt=getopt_long(argc, argv, optstring, &options[0], &optionIndex)) != -1 ) {
//	while ( (opt=getopt_long_only(argc, argv, optstring, longOptions, &optionIndex)) != -1 ) {
		if (opt == '?') {
			cout << char(optopt) << " not exists." << endl;
			continue;
		}
		printf("opt = %c\n", opt);
		printf("optarg = %s\n", optarg);
		printf("optind = %d\n", optind);
		printf("argv[optind-1] = %s\n", argv[optind-1]);
		printf("optindIndex = %d\n", optionIndex);
		cout << "-------------------" << endl;
	}
	int index = optind;
	while (index < argc) {
		cout << argv[index++] << endl;
	}
	return 0;
}
