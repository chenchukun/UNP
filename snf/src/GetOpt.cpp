#include "GetOpt.h"
#include <iostream>

namespace snf
{

GetOpt::GetOpt(int argc, char **argv, const std::string &optString, int optErr, int startIndex)
	: argc(argc)
	, argv(argv)
	, optString(optString)
	, optErr(optErr)
	, startIndex(startIndex)
{
	this->longOptions.push_back(option{0, 0, 0, 0});
}

int GetOpt::getNextOpt()
{
	if (it != opts.end()) {
		currOpt = it->first;
		auto pit = it++;
		return pit->first;
	}
	return -1;
}

const std::string GetOpt::getOptArg()
{
	if (currOpt == -1) {
		return std::string("");
	}
	return opts[currOpt];
}

std::map<int, std::string> GetOpt::getOpts()
{
	return opts;
}

void GetOpt::setLongOpt(const std::string &lopt, char sopt, int optType)
{
	longStrings.push_back(lopt);
	this->longOptions.insert(longOptions.begin(), option{longStrings.back().c_str(), optType, NULL, sopt});
}

void GetOpt::debug()
{
	/*
	std::cout << longOptions.size() << std::endl;
	for(int i=0; i<longOptions.size(); ++i) {
		std::cout << longOptions[i].name << "  "
			      << longOptions[i].has_arg << "  "
				  << longOptions[i].flag << "  "
				  << longOptions[i].val << std::endl;
	}
	*/
}

int GetOpt::parse()
{
	int opt;
	int optionIndex;
	opterr = optErr;
	optind = startIndex;
	opts.clear();
	argvs.clear();
	while ( (opt=getopt_long(argc, argv, optString.c_str(), &longOptions[0], &optionIndex)) != -1 ) {
		if (opt=='?' || opt==':') {
			return -1;
		}
		opts[opt] = optarg==NULL?std::string(""):std::string(optarg);
	}
	for (int i=optind; i<argc; ++i) {
		argvs.push_back(std::string(argv[i]));
	}
	it = opts.begin();
	currOpt = -1;
	return 0;
}

}
