#include <iostream>
#include <unistd.h>
#include <poll.h>

int main() {
    struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO;  // 標準入力
    fds[0].events = POLLIN;

    int ret = poll(fds, 1, 5000);  // 5秒のタイムアウト

    if (ret == -1) {
        perror("poll");
    } else if (ret == 0) {
        std::cout << "Timeout!\n";
    } else {
        if (fds[0].revents & POLLIN) {
            std::cout << "stdin is readable\n";
        }
    }

    return 0;
}

