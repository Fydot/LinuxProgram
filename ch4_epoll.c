#include <sys/epoll.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_EVENTS 64


int main()
{
    int fd, epfd, ret, nr_events, i;
    struct epoll_event event;
    struct epoll_event *events;

    // 创建 epoll fd
    if ((epfd = epoll_create1(0))== -1) {
        perror("epoll_create1");
        return 1;
    }

    // 注册标准输入 fd=0
    event.data.fd = 0;
    event.events = EPOLLIN | EPOLLOUT;
    if ((ret = epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event)) == -1) {
        perror("epoll_ctl");
        return 1;
    }

    // 等待 epoll 事件
    events = malloc(sizeof(struct epoll_event) * MAX_EVENTS);
    if (!events) {
        perror("malloc");
        return 1;
    }

    nr_events = epoll_wait(epfd, events, MAX_EVENTS, -1);
    for (i = 0; i < nr_events; i++) {
        printf("event=%ld on fd=%d\n", events[i].events, events[i].data.fd);
    }

    return 0;
}