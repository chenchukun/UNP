#ifndef HTTP_CONTEXT_H
#define HTTP_CONTEXT_H
#include "Buffer.h"
#include "HttpRequest.h"
#include <iostream>

class HttpContext
{
public:
	HttpContext() { 
		state = 0;	
//		state = -1;  // 为啥在parse中老是会变成0
	}

	int parse(snf::Buffer *buffer);

	HttpRequest* getHttpRequest() {
		return &httpRequest;
	}

private:
	int parseRequestLine(std::string &line);

	int split(std::string &str, const std::string &sp, std::vector<std::string> &spv);

	HttpRequest httpRequest;

	int state;
};

#endif
