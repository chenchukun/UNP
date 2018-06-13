#include "include/spdlog/spdlog.h"
#include "include/spdlog/fmt/ostr.h"
#include <iostream>
using namespace std;

void basic()
{
    // 创建输出到标准输出的log,接收参数为模块名称
    std::shared_ptr<spdlog::logger> console = spdlog::stdout_logger_mt("console");
    // 设置全局日志级别
    spdlog::set_level(spdlog::level::info);
    console->error("error message");
    console->warn("warn message");
    console->info("info message");
    console->debug("debug message");

    /*
    for (int i=0; i<3; ++i) {
        console->info_if(i%2!=0, "i % 2 != 0");
    }
     */

    // 通过模块名获取log指针
    auto log = spdlog::get("console");
    log->info("spdlog::get()");

    // 创建一个输出到文件的日志
    auto file = spdlog::basic_logger_mt("file_log", "./basic_log.log");
    // 遇到warn及以上级别时,立马将缓存写入文件,调用std::fflush(fd)
    file->flush_on(spdlog::level::warn);
    file->info("info message");
    file->debug("debug message");

    // 关闭指定日志
    spdlog::drop("file");
    // 关闭所有日志
    spdlog::drop_all();

}

int main()
{
    basic();
    return 0;
}