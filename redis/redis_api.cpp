#include <iostream>
#include <memory>
#include <hiredis-vip/hiredis.h>
#include <errno.h>
#include <string>
#include <vector>

using namespace std;

void parseReplay(redisReply *reply)
{
    switch (reply->type) {
        // 回复了命令的执行状态,如OK
        case REDIS_REPLY_STATUS:
        {
            cout << "REDIS_REPLY_STATUS: ";
            if (reply->len > 0) {
                cerr << reply->str << endl;
            }
            break;
        }
        // 回复了一个错误信息
        case REDIS_REPLY_ERROR:
        {
            cerr << "REDIS_REPLY_ERROR: ";
            if (reply->len > 0) {
                cerr << reply->str << endl;
            }
            break;
        }
        // 回复一个整数
        case REDIS_REPLY_INTEGER:
        {
            cout << "REDIS_REPLY_INTEGER: " << reply->integer << endl;
            break;
        }
        // 回复一个nil对象,表示没有数据可以访问
        case REDIS_REPLY_NIL:
        {
            cout << "REDIS_REPLY_NIL" << endl;
            break;
        }
        // 回复一个string
        case REDIS_REPLY_STRING:
        {
            cout << "REDIS_REPLY_STRING: ";
            cout << "len = " << reply->len << ", str = " << reply->str << endl;
            break;
        }
        // 回复一个array
        case REDIS_REPLY_ARRAY:
        {
            cout << "REDIS_REPLAY_ARRAY: " << endl;
            for (size_t i=0; i<reply->elements; ++i) {
                parseReplay(reply->element[i]);
            }
            break;
        }
    }
}

void command(redisContext *context, const string &strCmd)
{
    // 执行一个命令,失败返回NULL,失败后redisContext不能继续使用,需重新连接服务端
    redisReply *reply =  static_cast<redisReply*>(redisCommand(context, strCmd.c_str()));
    if (reply == NULL) {
        cerr << "Execute command failed: " << context->errstr << endl;
        switch (context->err) {
            case REDIS_ERR_IO:  // IO错误
                cout << "REDIS_ERR_IO: " << strerror(errno) << endl;
                break;
            case REDIS_ERR_EOF: // 服务器关闭,读取到EOF
                cout << "REDIS_ERR_EOF" << endl;
                break;
            case REDIS_ERR_PROTOCOL:    // 解析redis协议出错
                cout << "REDIS_ERR_PROTOCOL" << endl;
                break;
            case REDIS_ERR_OTHER:   // 其他错误
                cout << "REDIS_ERR_OTHER" << endl;
                break;
        }
        return;
    }
    parseReplay(reply);
    cout << "------------------------------------------------------------" << endl;
    freeReplyObject(reply);
}

void pipe(redisContext *context, const vector<string> &cmds)
{
    // redisAppendComand会将命令放入命令缓冲区
    for (int i=0; i<cmds.size(); ++i) {
        int ret = redisAppendCommand(context, cmds[i].c_str());
        if (ret == REDIS_ERR) {
            cerr << "redisAppendCommand failed: " << context->errstr << endl;
        }
    }
    // 调用redisGetReply,若结果缓冲区有结果则取出结果后立即返回。
    // 若结果缓冲区没有结果,则将命令缓冲区发送到服务器,并等待结果返回
    // redisCommand内部也是调用redisAppendCommand和redisGetReply函数
    // 执行redisGetReply的次数必须与执行redisAppendCommand的次数一直,否则可能会出现不可预料的错误
    for (int i=0; i<cmds.size(); ++i) {
        redisReply *reply = NULL;
        int ret = redisGetReply(context, reinterpret_cast<void**>(&reply));
        if (ret == REDIS_ERR) {
            cerr << "redisGetReply failed: " << context->errstr << endl;
        }
        else
        {
            parseReplay(reply);
        }
        freeReplyObject(reply);
    }
    cout << "------------------------------------------------------------" << endl;
}

// 同步API
void sync()
{
    // 创建连接,失败返回NULL,或者err为非0, redisContext为非线程安全
    redisContext *context = redisConnect("192.168.0.219", 6379);
    // 自定义超时时间
    /*
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    redisContext *context = redisConnectWithTimeout("192.168.0.219", 6379, &tv);
    */
    if (context == NULL || context->err != 0) {
        cerr << "Connect error: " << (context?context->errstr:"")
             << " [" << (context?context->err:-1) << "]" << endl;
        if (context) {
            redisFree(context);
        }
        exit(-1);
    }

    command(context, "keys *");
    command(context, "set test 123");
    command(context, "get test");
    command(context, "geft test");
    command(context, "incr test");

    pipe(context, {"set a 1", "get a", "incr a"});

    redisFree(context);
}

void async()
{

}

int main(int argc, char **argv)
{
    sync();
//    async();

    return 0;
}