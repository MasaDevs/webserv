#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

int main() {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        return 1;
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        perror("epoll_ctl");
        close(epoll_fd);
        return 1;
    }

    struct epoll_event events[1];
    int ret = epoll_wait(epoll_fd, events, 1, 5000);  // 5秒のタイムアウト

    if (ret == -1) {
        perror("epoll_wait");
    } else if (ret == 0) {
        std::cout << "Timeout!\n";
    } else {
        std::cout << "stdin is readable\n";
    }

    close(epoll_fd);
    return 0;
}

