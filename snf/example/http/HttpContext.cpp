#include "HttpContext.h"

int HttpContext::parse(snf::Buffer *buffer) {
	int index = buffer->findCRLF();
	if (state == 0) {
		HttpRequest request;
		httpRequest = request;
	}

	while (index >= 0) {
		// 读取到\r\n
		if (index == 0) {
			buffer->discard(2);
			// 当前状态为读取为头部信息
			if (state == 1) {
				// 若是GET请求，则请求读取完成
				if (httpRequest.getMethod() == "GET") {
					state = 0;
					buffer->discardAll();
					return 0;
				}
				// 否则更改状态为开始读取请求体
				else {
					std::string content;
					buffer->readAll(content);
					httpRequest.setQuery(content);
					return 0;
				}
			}
			// 当前状态为读取请求体
			else if (state == 2) {
				state = 0;
				return 0;
			}
			// \r\n出现在了不该出现的地方，请求解析失败
			else {
				state = 0;
				return -1;
			}
		}
		std::string line;
		buffer->read(line, index);
		buffer->discard(2);

		if (state == 0) {
			int ret = parseRequestLine(line);
			if (ret == -1) {
				state = 0;
				return -1;
			}
			state = 1;
		}
		else if (state == 1) {
			int last = line.find_first_of(":");
			if (last == std::string::npos || last>=line.size()-1) {
				state = 0;
				return -1;
			}
			httpRequest.setHeader(line.substr(0, last), line.substr(last+1, line.size()-last-1));
		}
		index = buffer->findCRLF();
	}
	return 1;
}

int HttpContext::split(std::string &str, const std::string &sp, std::vector<std::string> &spv)
{
	int index = 0;
	int start = 0;
	while ((index=str.find_first_of(sp, start)) != std::string::npos) {
		spv.push_back(str.substr(start, index-start));
		start = index + 1;
	}
	if (start < str.size()) {
		spv.push_back(str.substr(start, str.size()-start));
	}
	return spv.size();
}

int HttpContext::parseRequestLine(std::string &line)
{
	std::vector<std::string> spv;
	int n = split(line, " ", spv);
	if (n != 3) {
		return -1;
	}
	httpRequest.setMethod(spv[0]);
	httpRequest.setVersion(spv[2]);
	std::string &path = spv[1];
	int index = path.find_first_of("?"); 
	if (index == std::string::npos) {
		httpRequest.setPath(path);
	}
	else {
		httpRequest.setPath(path.substr(0, index));
		std::string query = path.substr(index+1, path.size()-index-1);
		httpRequest.setQuery(query);
	}
	return 0;
}


