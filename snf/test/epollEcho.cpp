#include <iostream> 
#include <sys/socket.h>  
#include <sys/epoll.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <stdio.h>  
#include <errno.h>  
#include <stdlib.h>
#include <string.h>


using namespace std; 
 
#define MAXLINE 512  
#define OPEN_MAX 100  
#define LISTENQ 20  
#define SERV_PORT  6180
#define INFTIM 1000  
 
void setnonblocking(int sock)  
{  
    int opts;  
    opts=fcntl(sock,F_GETFL);  
 
    if(opts<0)  
    {  
        perror("fcntl(sock,GETFL)");  
        exit(1);  
    }  
 
    opts = opts | O_NONBLOCK;  
 
    if(fcntl(sock,F_SETFL,opts)<0)  
    {  
        perror("fcntl(sock,SETFL,opts)");  
        exit(1);  
    }  
}  
 
ssize_t socket_send(int sockfd, const char* buffer, size_t buflen) 
{ 
    ssize_t tmp; 
    size_t total = buflen; 
    const char *p = buffer; 
 
    while(1) 
    { 
        tmp = send(sockfd, p, total, 0); 
        if(tmp < 0) 
        { 
            // 当send收到信号时,可以继续写,但这里返回-1. 
            if(errno == EINTR) 
                //return -1;zxd 
                continue; 
 
            // 当socket是非阻塞时,如返回此错误,表示写缓冲队列已满, 
            // 在这里做延时后再重试. 
            if(errno == EAGAIN) 
            { 
                usleep(1000); 
                continue; 
            } 
 
            return -1; 
        } 
 
        if((size_t)tmp == total) 
            return buflen; 
 
        total -= tmp; 
        p += tmp; 
    } 
 
    return tmp; 
} 
 
 
void epoll_ctl_err_show()   
{   
    std::cout << "error at epoll_ctl" << std::endl;   
    if(EBADF == errno)   
    {   
        std::cout << "error at epoll_ctl, error  is EBADF" << std::endl;   
    }   
    else if(EEXIST == errno)   
    {   
        std::cout << "error at epoll_ctl, error  is EEXIST" << std::endl;   
    }   
    else if(EINVAL == errno)   
    {   
        std::cout << "error at epoll_ctl, error  is EINVAL" << std::endl;   
    }   
    else if(ENOENT == errno)   
    {   
        std::cout << "error at epoll_ctl, error  is ENOENT" << std::endl;   
    }   
    else if(ENOMEM == errno)   
    {   
        std::cout << "error at epoll_ctl, error  is ENOMEM" << std::endl;   
    }   
    else if(ENOSPC == errno)   
    {   
        std::cout << "error at epoll_ctl, error  is ENOSPC" << std::endl;   
    }   
      
}  
 
int main()  
{  
    int i, maxi, listenfd, connfd, sockfd, epfd, nfds;  
    ssize_t n;  
    char line[MAXLINE];  
    socklen_t clilen;  
 
    struct epoll_event ev,events[20]; //声明epoll_event结构体的变量, ev用于注册事件, events数组用于回传要处理的事件  
    epfd=epoll_create(256); //生成用于处理accept的epoll专用的文件描述符, 指定生成描述符的最大范围为256  
 
    struct sockaddr_in clientaddr;  
    struct sockaddr_in serveraddr;  
 
    listenfd = socket(AF_INET, SOCK_STREAM, 0);  
 
    setnonblocking(listenfd); //把用于监听的socket设置为非阻塞方式  
 
    ev.data.fd=listenfd; //设置与要处理的事件相关的文件描述符  
    ev.events=EPOLLIN | EPOLLET; //设置要处理的事件类型  
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev); //注册epoll事件 
 
    bzero(&serveraddr, sizeof(serveraddr));  
    serveraddr.sin_family = AF_INET;  
    const char *local_addr="0.0.0.0";  
    inet_aton(local_addr,&(serveraddr.sin_addr)); 
    serveraddr.sin_port=htons(SERV_PORT);  //或者htons(SERV_PORT);  
 
    bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));  
 
    listen(listenfd, LISTENQ);  
 
    maxi = 0;  
 
    for( ; ; ) {  
        nfds=epoll_wait(epfd, events, 20, -1); //等待epoll事件的发生  
         
 
        for(i=0;i<nfds;++i) //处理所发生的所有事件  
        {  
            if(events[i].data.fd==listenfd)    /**监听事件**/ 
            {  
                //循环accept 
                while((connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen)) > 0) 
                { 
                    setnonblocking(connfd); //把客户端的socket设置为非阻塞方式 
 
                    char *str = inet_ntoa(clientaddr.sin_addr);  
                    std::cout<<"connect from "<<str<<std::endl;  
 
                    ev.data.fd=connfd; //设置用于读操作的文件描述符  
                    ev.events=EPOLLIN | EPOLLET; //设置用于注测的读操作事件  
                    epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev); //注册ev事件  
                } 
            }  
            else if(events[i].events&EPOLLIN)     /**读事件**/ 
            {  
                //fprintf(stderr, "EPOLLIN................%d\n", maxi++); 
                if ( (sockfd = events[i].data.fd) < 0)  
                { 
                    continue;  
                } 
                memset(line, 0, MAXLINE); 
                n = 0; 
                int nread = 0; 
                while((nread= read(sockfd, line + n, MAXLINE)) > 0) 
                { 
                    n += nread; 
                }//读到EAGAIN，说明读完了 
 
 
                if(nread == -1 && errno != EAGAIN) 
                { 
                    epoll_ctl_err_show(); 
                    std::cout<<"readline error"<<std::endl; 
                    close(sockfd); //关闭一个文件描述符，那么它会从epoll集合中自动删除 
                    //描述符关闭后，后面的邋邋邋邋EPOLLOUT设置了，但不起作用了 
                    events[i].data.fd = -1;  
                } 
 
                //这里要加上判断，nread为0时，说明客户端已经关闭 
                //此时，需要关闭描述符，否则在/proc/id/fd下能看到描述符会一直存在 
                if(nread == 0) 
                { 
                    close(sockfd); 
                    continue; 
                } 
 
 
                ev.data.fd=sockfd; //设置用于写操作的文件描述符  
                ev.events=EPOLLOUT | EPOLLET; //设置用于注测的写操作事件  
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev); //修改sockfd上要处理的事件为EPOLLOUT  
            }  
            else if(events[i].events & EPOLLOUT)    /**写事件**/ 
            {  
                sockfd = events[i].data.fd;  
                //write(sockfd, line, n); orig 
 
                int iRet = socket_send(sockfd, line, strlen(line) + 1); 
                if(iRet == -1 || iRet != strlen(line) + 1) 
                { 
                    perror("write error!"); 
                }/*zxd*/ 
 
                ev.data.fd=sockfd; //设置用于读操作的文件描述符  
                ev.events=EPOLLIN | EPOLLET; //设置用于注册的读操作事件  
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev); //修改sockfd上要处理的事件为EPOLIN  
            }  
        }  
	}
}
