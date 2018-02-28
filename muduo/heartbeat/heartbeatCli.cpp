//
// Created by chenchukun on 18/2/28.
//
#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;

enum {HEAD_LEN = 4};

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: heartbeatCli IP\n");
        exit(1);
    }
    EventLoop loop;
    InetAddress address(argv[1], 6180);
    TcpClient client(&loop, address, "heartbeat");
    client.setConnectionCallback([&loop, &client] (const TcpConnectionPtr &conn) {
        if (conn->connected()) {
            LOG_INFO << "connect to " << conn->peerAddress().toIpPort() << " success";
            loop.runEvery(5, [conn] {
                Buffer hbmsg;
                hbmsg.append("heartbeat", 9);
                hbmsg.prependInt32(9);
                conn->send(&hbmsg);
            });
        }
        else {
            LOG_INFO << "disconnect " << conn->peerAddress().toIpPort();
            client.connect();
        }
    });
    client.setMessageCallback([] (const TcpConnectionPtr &conn, Buffer *buff, Timestamp timestamp) {
        while (buff->readableBytes() > HEAD_LEN) {
            int32_t len = buff->peekInt32();
            if (buff->readableBytes() >= len + HEAD_LEN) {
                buff->retrieve(HEAD_LEN);
                std::string msg(buff->peek(), len);
                LOG_INFO << "recv " << msg << " from " << conn->peerAddress().toIpPort();
                buff->retrieve(len);
            }
            else {
                break;
            }
        }
    });
    client.connect();
    loop.loop();
    return 0;
}