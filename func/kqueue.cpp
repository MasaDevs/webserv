
#include <iostream>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
int kq = kqueue();
if (kq == -1) {
    // エラー処理
}

struct kevent changes[1];
struct kevent events[1];

// イベントを設定
EV_SET(&changes[0], some_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);

// イベントの登録
int ret = kevent(kq, changes, 1, events, 1, NULL);
if (ret == -1) {
    // エラー処理
} else if (ret > 0) {
    // イベントを処理
}

int main() {
    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        return 1;
    }

    struct kevent change;
    struct kevent event;

    // 標準入力の読み取りを監視
    EV_SET(&change, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);

    // イベントの変更を登録
    int ret = kevent(kq, &change, 1, &event, 1, NULL);
    if (ret == -1) {
        perror("kevent");
        close(kq);
        return 1;
    } else if (ret > 0) {
        std::cout << "stdin is readable" << std::endl;
    }

    close(kq);
    return 0;
}

