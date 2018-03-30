//
// Created by chenchukun on 18/3/30.
//
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    string data((const char*) ptr, (size_t) size * nmemb);

    *((stringstream*) stream) << data << endl;

    return size * nmemb;
}

void get(string url)
{
    CURL *curl = curl_easy_init();
    CURLcode res;
    std::stringstream out;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: okhttp");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        res = curl_easy_perform(curl);
        cout << curl_easy_strerror(res) << endl;
        cout << out.str() << endl;
    }
    curl_easy_cleanup(curl);
}

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);

    string url = "http://www.baidu.com";
    get(url);
    return 0;
}