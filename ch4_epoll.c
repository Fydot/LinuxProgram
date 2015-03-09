#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>

#define MAX_EVENTS 64
#define MAX_BUFSIZ 1024


int main()
{
    int fd, epfd, ret, nr_events, i, len;
    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];
    char buf[MAX_BUFSIZ];

    // 创建 epoll fd
    if ((epfd = epoll_create1(0))== -1) {
        perror("epoll_create1");
        return 1;
    }

    // 注册标准输入 fd=0
    event.data.fd = 0;
    event.events = EPOLLIN;
    if ((ret = epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event)) == -1) {
        perror("epoll_ctl");
        return 1;
    }

    // 等待 epoll 事件
    while(1) {
        nr_events = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (i = 0; i < nr_events; i++) {
            if (events[i].events == EPOLLIN) {
                len = read(events[i].data.fd, buf, MAX_BUFSIZ);
                buf[len] = '\0';
                printf("buf = \"%s\"", buf);
            }
        }
    }

    return 0;
}
