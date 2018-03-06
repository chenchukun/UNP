#include "HttpServer.h"
#include "Reactor.h"
#include <fstream>
#include <iterator>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace snf;
using namespace std;

string workDir = "/home/chenchukun/snf/example/http/workDir";

int getFileContent(const string &path, string &content) 
{
	ifstream fin(workDir + path);	
	if (!fin) {
		return -1;
	}
	istreambuf_iterator<char> beg(fin), end; 
	content = string(beg, end);
}

int execcuteCgi(const string &path, const string &method, const string &query, string &content)
{
	int cgiInput[2];
	int cgiOutput[2];
	if (pipe(cgiInput) < 0) {
		return -1;
	}
	if (pipe(cgiOutput) < 0) {
		return -1;
	}
	pid_t pid = fork();
	if (pid < 0) {
		return -1;
	}
	if (pid == 0) {
		dup2(cgiInput[0], 0);
		dup2(cgiOutput[1], 1);
		close(cgiInput[1]);
		close(cgiOutput[0]);
		string methEnv = "REQUEST_METHOD=" + method;
		putenv(const_cast<char*>(methEnv.c_str()));
		if ( method == "GET") {
			string queryEnv = "QUERY_STRING=" + query;
			putenv(const_cast<char*>(queryEnv.c_str()));
		}
		else {
			string lengthEnv = "CONTENT_LENGTH=" + to_string(query.size());
			putenv(const_cast<char*>(lengthEnv.c_str()));
		}
		string cmd = workDir + path;
		execl(cmd.c_str(), cmd.c_str(), NULL);
	}
	else {
		close(cgiInput[0]);
		close(cgiOutput[1]);
		if (method == "POST") {
			int len = write(cgiInput[1], query.c_str(), query.size());
			assert(len == query.size());
		}
		int len = 0;
		char buff[128];
		while ((len = read(cgiOutput[0], buff, sizeof(buff))) > 0) {
			string text = string(buff, 0, len);
			content += text;
		}
		waitpid(pid, NULL, 0);
		close(cgiInput[1]);
		close(cgiOutput[0]);
	}
	return 0;
}

void onRequest(HttpRequest *request, HttpResponse &response) 
{
	string method = request->getMethod();
	string path = request->getPath();
	if (path == "/") {
		path += "index.html";
	}
	const string &query = request->getQuery();
	string content;
	int ret = 0;
	if (method == "GET" && query.size()==0) {
		ret = getFileContent(path, content);
	}
	else {
		ret = execcuteCgi(path, method, query, content);
	}
	if (ret == -1) {
		response.setVersion("HTTP/1.1");
		response.setStatusCode(404);
		response.setHeaders(request->getHeaders());
		response.setBody("Not Found");
	}
	else {
		response.setVersion("HTTP/1.1");
		response.setStatusCode(200);
		response.setHeaders(request->getHeaders());
		response.setBody(content);
	}
}

int main()
{
	Reactor reactor;
	SockAddr addr("0.0.0.0", 8080);
	HttpServer httpServer(&reactor, addr);
	httpServer.setRequestCallback(onRequest);
	httpServer.start();
	reactor.run();

	return 0;
}
