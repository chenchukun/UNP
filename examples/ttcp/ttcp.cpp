//
// Created by chenchukun on 17/11/19.
//
#include <iostream>
#include <string>
#include <getopt.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <sys/time.h>
using namespace std;

#pragma pack(1)

enum Type{TTCP_SEND, TTCP_RECV};

struct Option
{
    string host;
    short port;
    Type type;
    int32_t length;
    int32_t number;
    Option()
        : host(""), port(0), type(TTCP_RECV),
          length(0), number(0)
    {
    }
};

struct SessionMessage
{
    int32_t number;
    int32_t length;
};

struct PayloadMessage
{
    int32_t length;
    char data[0];
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

int readn(int fd, void *buff, int len)
{
    int offset = 0;
    int nread = 0;
    char *ptr = (char *) buff;
    while ((nread = read(fd, ptr+offset, len-offset)) >= 0) {
        if (nread == 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
        offset += nread;
    }
    if (nread < 0) {
        return -1;
    }
    return offset;
}

int writen(int fd, const void *buff, int len)
{
    int nwrite = 0;
    int offset = 0;
    const char *ptr = (const char *) buff;
    while (offset<len && (nwrite = write(fd, ptr+offset, len-offset)) >= 0) {
        if (nwrite == 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
        offset += nwrite;
    }
    if (nwrite < 0) {
        return -1;
    }
    return offset;
}

void recv(const Option &option)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd>=0);
    int yes = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {
        perror("setsockopt");
        exit(1);
    }
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    if (option.host.empty()) {
        addr.sin_addr.s_addr = INADDR_ANY;
    }
    else {
        inet_pton(AF_INET, option.host.c_str(), &addr.sin_addr);
    }
    addr.sin_port = htons(option.port);
    if (bind(listenfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr))) {
        perror("bind");
        exit(1);
    }
    if (listen(listenfd, 1)) {
        perror("listen");
        exit(1);
    }
    int sockfd = accept(listenfd, NULL, NULL);
    if (sockfd < 0) {
        perror("accept");
        exit(1);
    }
    close(listenfd);
    SessionMessage sessionMessage;
    int n = readn(sockfd, &sessionMessage, sizeof(sessionMessage));
    if (n != sizeof(sessionMessage)) {
        perror("read sessionMessage");
        exit(1);
    }
    sessionMessage.number = ntohl(sessionMessage.number);
    sessionMessage.length = ntohl(sessionMessage.length);
    int msgLen = sessionMessage.length + sizeof(int32_t);
    PayloadMessage *payloadMessage = reinterpret_cast<PayloadMessage*>(malloc(msgLen));
    for (int i=0; i<sessionMessage.number; ++i) {
        n = readn(sockfd, &payloadMessage->length, sizeof(int32_t));
        assert(n == sizeof(int32_t));
        payloadMessage->length = ntohl(payloadMessage->length);
        assert(payloadMessage->length == sessionMessage.length);
        n = readn(sockfd, payloadMessage->data, sessionMessage.length);
        assert(n == sessionMessage.length);
        int32_t ack = htonl(sessionMessage.length);
        n = writen(sockfd, &ack, sizeof(ack));
        assert(n == sizeof(ack));
    }
    free(payloadMessage);
    close(sockfd);
}

void send(const Option &option)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd>=0);
    struct hostent* he = gethostbyname(option.host.c_str());
    if (!he)
    {
        perror("gethostbyname");
        exit(1);
    }
    assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
    addr.sin_port = htons(option.port);
    if (connect(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr))) {
        perror("connect");
        exit(1);
    }
    const char *msg = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    SessionMessage sessionMessage;
    sessionMessage.number = htonl(option.number);
    sessionMessage.length = htonl(option.length);
    int n = writen(sockfd, &sessionMessage, sizeof(sessionMessage));
    assert(n == sizeof(sessionMessage));
    int msgLen = option.length + sizeof(int32_t);
    PayloadMessage *payloadMessage = reinterpret_cast<PayloadMessage*>(malloc(msgLen));
    payloadMessage->length = htonl(option.length);
    double total = 1.0*option.length*option.number / 1024 / 1024;
    cout << "send " << total << " MB" << endl;
    timeval startTime, endTime;
    gettimeofday(&startTime, NULL);
    for (int i=0; i<option.length; ++i) {
        payloadMessage->data[i] = msg[i%strlen(msg)];
    }
    for (int i=0; i<option.number; ++i) {
        n = writen(sockfd, payloadMessage, msgLen);
        assert(n == msgLen);
        int32_t ack = 0;
        n = readn(sockfd, &ack, sizeof(ack));
        assert(n == sizeof(ack));
        ack = ntohl(ack);
        assert(ack == option.length);
    }
    gettimeofday(&endTime, NULL);
    int64_t etimestamp = endTime.tv_sec*1000000 + endTime.tv_usec;
    int64_t stimestamp = startTime.tv_sec*1000000 + startTime.tv_usec;
    double ttimestamp = 1.0*(etimestamp - stimestamp)/1000000;
    cout << "take:\t" << ttimestamp << "s" << endl;
    cout << "speed:\t" << total/ttimestamp << "MB/s" << endl;
    free(payloadMessage);
    close(sockfd);
}

int main(int argc, char **argv)
{
    Option option;
    parseCommandLine(argc, argv, option);
    if (option.type == TTCP_RECV) {
        recv(option);
    }
    else {
        send(option);
    }
    return 0;
}