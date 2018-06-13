#include "include/spdlog/fmt/fmt.h"
#include <string>
using namespace std;


int main()
{
    // fmt::print输出格式化字符串到标准输出,格式化语法和python的format类似
    fmt::print("hello {}\n", "world");

    // fmt::format生成一个std::string
    string str = fmt::format("{0}{1}{0}", "'", "hello");
    fmt::print("{}\n", str);

    return 0;
}