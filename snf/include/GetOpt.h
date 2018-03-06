#ifndef GETOPT_H
#define GETOPT_H
#include <getopt.h>
#include <string>
#include <vector>
#include <map>

namespace snf
{

class GetOpt
{
public:
	enum {REQUIRED_ARG=required_argument, NO_ARG=no_argument, OPTIONAL_ARG=optional_argument};

	GetOpt(int argc, char **argv, const std::string &optString, int optErr=0, int startIndex=1);

	int getNextOpt();

	const std::string getOptArg();

	void setLongOpt(const std::string &lopt, char sopt, int optType);

	int parse();

	std::map<int, std::string> getOpts();

	void debug();

private:
	int argc;

	char **argv;

	std::string optString;

	int optErr;

	int startIndex;

	std::vector<struct option> longOptions;

	std::map<int, std::string> opts;

	std::vector<std::string> argvs;

	std::vector<std::string> longStrings;

	std::map<int, std::string>::iterator it;

	int currOpt;

};

}

#endif
