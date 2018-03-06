#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H
#include <string>
#include <map>

class HttpResponse
{
public:
	void setVersion(const std::string &ver) {
		version = ver;
	}

	std::string getVersion() const {
		return version;
	}

	void setStatusCode(int code) {
		statusCode = code;
		switch (statusCode) {
			case 200:
				statusInfo = "OK";
				break;
			case 400:
				statusInfo = "Bad Request";
				break;
			case 404:
				statusInfo = "Not Found";
				break;
			default:
				statusCode = 403;
				statusInfo = "Forbidden";
				break;
		}
	}

	int getStatusCode() {
		return statusCode;
	}

	std::string getStatusInfo() const {
		return statusInfo;
	}

	void setHeader(const std::string &key, const std::string &value) {
		headers[key] = value;
	}

	void setHeaders(const std::map<std::string, std::string> &headers) {
		this->headers = headers;
	}

	std::string getHeader(const std::string &key) {
		if (headers.count(key) <= 0) {
			return "";
		}
		return headers[key];
	}

	const std::map<std::string, std::string>& getHeaders() const {
		return headers;
	}

	void setBody(const std::string &body) {
		this->body = body;
	}

	void setBody(const std::string &&body) {
		this->body = body;
	}

	const std::string& getBody() const {
		return body;
	}

	int serialize(std::string &content) {
		if (version!="HTTP/1.0" && version!="HTTP/1.1") {
			return -1;
		}
		if (statusCode!=200 && statusCode!=400 && statusCode!=404 && statusCode!=403) {
			return -1;
		}
		content += version + " " + std::to_string(statusCode) + " " + statusInfo + "\r\n";
		auto it = headers.cbegin();
		while (it != headers.cend()) {
			content += it->first + ": " + it->second + "\r\n";
			++it;
		}
		content += "\r\n";
		content += body;
		return 0;
	}

private:
	std::string version;
	
	int statusCode;

	std::string statusInfo;

	std::map<std::string, std::string> headers;

	std::string body;
};

#endif
