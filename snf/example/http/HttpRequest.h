#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
#include <string>
#include <map>

class HttpRequest
{
public:
	HttpRequest() {}

	void setMethod(const std::string &method) {
		this->method = method;
	}

	std::string getMethod() const {
		return method;
	}

	void setPath(const std::string &path) {
		this->path = path;
	}

	std::string getPath() const {
		return path;
	}

	void setVersion(const std::string &version) {
		this->version = version;
	}

	std::string getVersion() const {
		return version;
	}

	void setQuery(const std::string query) {
		this->query = query;
	}

	const std::string& getQuery() const {
		return query;
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

private:
	std::string method;

	std::string path;

	std::string version;

	std::string query;

	std::map<std::string, std::string> headers;
};

#endif
