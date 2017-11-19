//
// Created by chenchukun on 17/11/19.
//
#include <iostream>
#include <string>
#include <getopt.h>
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;

enum Type{TTCP_SEND, TTCP_RECV};

struct Option {
    string host;
    short port;
    Type type;
    int length;
    int number;
    Option()
        : host(""), port(0), type(TTCP_RECV),
          length(0), number(0)
    {
    }
};

void help() {
    cout << "ttcp -b ip -p port -t SEND|RECV [-l length] [-n number]" << endl;
    exit(0);
}

Type parseType(const char *optarg)
{
    if (strcasecmp(optarg, "SEND") == 0) {
        return TTCP_SEND;
    }
    if (strcasecmp(optarg, "RECV") == 0) {
        return TTCP_RECV;
    }
    return TTCP_RECV;
}

void parseCommandLine(int argc, char **argv, Option &option)
{
    int opt;
    while ((opt = getopt(argc, argv, "b:p:t:l:n:h")) != -1) {
        switch (opt) {
        case 'b':
            option.host = optarg;
            break;
        case 'p':
            option.port = stoi(string(optarg));
            break;
        case 't':
            option.type = parseType(optarg);
            break;
        case 'l':
            option.length = stoi(string(optarg));
            break;
        case 'n':
            option.number = stoi(string(optarg));
            break;
        case 'h':
        default:
            help();
            break;
        }
    }
    if (option.type == TTCP_SEND) {
        if (option.host.empty() || option.port==0 || option.number==0
            || option.length==0)
        {
            help();
            exit(0);
        }
    }
    if (option.type == TTCP_RECV && option.port==0) {
        help();
        exit(0);
    }
}

int main(int argc, char **argv)
{
    Option option;
    parseCommandLine(argc, argv, option);
    return 0;
}