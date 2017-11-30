//
// Created by chenchukun on 17/11/30.
//

#include <sys/socket.h>
#include <iostream>
#include <memory>
#include <assert.h>
#include<sys/wait.h>
#include "unp.h"
using namespace std;

#define EXECL


void newProcess()
{
    // 通过getppid()获取父进程PID
    cout << "进程" << getppid() << " 的子进程开始运行" << endl;
    // 6个exec函数中,只有execv是内核中的系统调用,其他函数都会转为execv

#ifdef EXECL
    // execl 指定可执行文件路径,需要列出所有参数,参数要包含命令
    int ret = execl("/bin/ls", "/bin/ls", "-l", "/", NULL);
    if (ret == -1) {
        cerr << "调用execl失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
#endif

#ifdef EXECV
    // execl 指定可执行文件路径,参数通过数组形式给出
    char* argv[] = {"/bin/ls", "-l", "/", NULL};
    // execv的参数设计是不是不合理?应该是const char * const argv[]比较合理
    int ret = execv("/bin/ls", argv);
    if (ret == -1) {
        cerr << "调用execv失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
#endif

#ifdef EXECLE
    cout << "environ = {";
    for (int i=0; environ[i] != NULL; ++i) {
        cout << "\t" << environ[i] << endl;
    }
    cout << "}" << endl;
    char* env[] = {"PATH=/usr/bin;/bin", NULL};
    // execle 指定可执行文件路径,需要列出所有参数,需要指定传递给新进程的环境变量
//    int ret = execle("/bin/echo", "$PATH", NULL, environ);
    int ret = execle("/usr/bin/env", "/usr/bin/env", NULL, env);
    if (ret == -1) {
        cerr << "调用execle失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
#endif

#ifdef EXECVE
    cout << "environ = {";
    for (int i=0; environ[i] != NULL; ++i) {
        cout << "\t" << environ[i] << endl;
    }
    cout << "}" << endl;
    char* env[] = {"PATH=/usr/bin;/bin", NULL};
    // execle 指定可执行文件路径,需要列出所有参数,需要指定传递给新进程的环境变量
//    int ret = execle("/bin/echo", "$PATH", NULL, environ);
    char *argv[] = {"/usr/bin/env", NULL};
    int ret = execve("/usr/bin/env", argv, env);
    if (ret == -1) {
        cerr << "调用exeve失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
#endif

#ifdef EXECLP
    // execlp 不需要指定路径,execlp会使用PATH拼接路径并转为调用execve,需要列出所有参数
    int ret = execlp("ls", "ls", "-l", "/", NULL);
    if (ret == -1) {
        cerr << "调用execlp失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
#endif

#ifdef EXECVP
    // execvp 不需要指定路径,参数通过数组形式给出
    char* argv[] = {"ls", "-l", "/", NULL};
    int ret = execvp("ls", argv);
    if (ret == -1) {
        cerr << "调用execvp失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
#endif
}

int main(int argc, char **argv)
{
    // fork()是Unix中产生新进程的唯一方法
    // fork()返回两次,子进程返回0,父进程返回子进程PID,失败返回-1
    // 父进程获取子进程PID的唯一途径就是通过fork()的返回值获取。
    pid_t pid = fork();
    switch (pid) {
        case 0:
            newProcess();
            break;
        case -1:
            cerr << "创建子进程失败:" << strerror(errno) << "(" << errno << ")" << endl;
            break;
        default:
            cout << "创建子进程成功, 子进程pid = " << pid << endl;
            wait(NULL);
    }
    return 0;
}