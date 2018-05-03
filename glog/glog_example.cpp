#include <glog/logging.h>
#include <gflags/gflags.h>
#include <iostream>

using namespace std;

DEFINE_bool(print, false, "test LOG_IF");

int main(int argc, char **argv)
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::GLOG_INFO, "./example.log");
    // 设置最大日志文件大小
    FLAGS_max_log_size = 20;
    // 不输出到文件,输出到stderr
    FLAGS_logtostderr = true;
    // 错误日志输出有颜色日志
    FLAGS_colorlogtostderr=true;

    // glog支持的日志级别INFO,WARNING, ERROR, FATAL, FATAL日志会在打印后终止程序
    LOG(INFO) << "this is glog example";

    // 条件输出: 当添加为真时输出日志,否则不输出
    LOG_IF(INFO, FLAGS_print) << "print log_if";

    for (int i=0; i<5; ++i) {
        // 前N次打印日志
        LOG_FIRST_N(INFO, 3) << "log_first_n i = " << i;
        // 在首次打印日志后,每个N次打印日志
        LOG_EVERY_N(INFO, 2) << "log_every_n i = " << i;
    }
    // ERROR和FATAL默认情况下会输出到stderr
    LOG(ERROR) << "print error";

    // DUBUG日志,定义NDEBUG宏后不会打印该日志
    DLOG(INFO) << "print dlog";

    google::ShutdownGoogleLogging();

    // FATAL日志默认会打印出堆栈信息
    LOG(FATAL) << "print fatal";

    return 0;
}